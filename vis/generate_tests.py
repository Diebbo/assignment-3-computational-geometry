import os
import random
from dataclasses import dataclass
from typing import Callable, Tuple
import numpy as np


@dataclass
class ShapeConfig:
    """Configuration for a shape's bounds and generation function."""
    name: str
    generator: Callable[[float], float]
    get_bounds: Callable[[float, int], Tuple[float, float]]


class TestDataGenerator:
    """Generates test data for different geometric shapes."""
    
    OUTPUT_DIR = "build/tests/"
    SIZE_RANGE = range(8, 20)
    SCALE_FACTOR = 10.0
    BASE_SIZE = 256
    PRECISION = 3
    
    def __init__(self):
        self.shapes = self._init_shapes()
    
    def _init_shapes(self) -> dict[str, ShapeConfig]:
        """Initialize shape configurations."""
        def parabola_gen(x: float) -> float:
            return x**2
        
        def square_gen(x: float, lower: float, upper: float) -> float:
            return np.random.uniform(lower, upper)
        
        def circle_gen(x: float, r: float) -> float:
            max_y = (r**2 - x**2) ** 0.5
            return np.random.uniform(-max_y, max_y)
        
        def standard_bounds(scale: float, size: int) -> Tuple[float, float]:
            scaled = scale * (size / self.BASE_SIZE)
            return (-scaled, scaled)
        
        def circle_bounds(scale: float, size: int) -> Tuple[float, float]:
            r = scale * (size / self.BASE_SIZE)
            return (-r, r)
        
        return {
            "parabola": ShapeConfig(
                name="parabola",
                generator=parabola_gen,
                get_bounds=standard_bounds
            ),
            "square": ShapeConfig(
                name="square",
                generator=lambda x, lower, upper: square_gen(x, lower, upper),
                get_bounds=standard_bounds
            ),
            "circle": ShapeConfig(
                name="circle",
                generator=lambda x, r: circle_gen(x, r),
                get_bounds=circle_bounds
            ),
        }
    
    def _generate_unique_x_values(self, size: int, lower: float, upper: float) -> list[float]:
        """Generate unique x values within bounds."""
        x_vals = set()
        while len(x_vals) < size:
            x = round(random.uniform(lower, upper), self.PRECISION)
            x_vals.add(x)
        return sorted(x_vals)
    
    def _generate_y_values(self, shape: str, x_vals: list[float], size: int) -> list[float]:
        """Generate y values based on shape type."""
        config = self.shapes[shape]
        lower, upper = config.get_bounds(self.SCALE_FACTOR, size)
        
        if shape == "parabola":
            return [config.generator(x) for x in x_vals]
        elif shape == "square":
            return [config.generator(x, lower, upper) for x in x_vals]
        elif shape == "circle":
            r = self.SCALE_FACTOR * (size / self.BASE_SIZE)  # radius grows with size
            return [config.generator(x, r) for x in x_vals]
        
        raise ValueError(f"Unknown shape: {shape}")
    
    def _write_data_file(self, filepath: str, size: int, x_vals: list[float], 
                        y_vals: list[float], shuffle: bool = False):
        """Write data to file, optionally shuffled."""
        data = list(zip(x_vals, y_vals))
        if shuffle:
            random.shuffle(data)
        
        with open(filepath, "w") as f:
            f.write(f"{size}\n")
            for x, y in data:
                f.write(f"{x:.{self.PRECISION}f} {y:.{self.PRECISION}f}\n")
    
    def generate_test_data(self):
        """Generate all test data files."""
        os.makedirs(self.OUTPUT_DIR, exist_ok=True)
        
        for shape in self.shapes.keys():
            shape_dir = os.path.join(self.OUTPUT_DIR, shape)
            os.makedirs(shape_dir, exist_ok=True)
            
            for i in self.SIZE_RANGE:
                size = 2**i
                print(f"Generating {shape} test with size {size}")
                
                # Get bounds for this shape and size
                lower, upper = self.shapes[shape].get_bounds(self.SCALE_FACTOR, size)
                
                # Generate data
                x_vals = self._generate_unique_x_values(size, lower, upper)
                y_vals = self._generate_y_values(shape, x_vals, size)
                
                # Write sorted and unsorted versions
                sorted_path = os.path.join(shape_dir, f"{size}_sorted")
                unsorted_path = os.path.join(shape_dir, str(size))
                
                self._write_data_file(sorted_path, size, x_vals, y_vals, shuffle=False)
                self._write_data_file(unsorted_path, size, x_vals, y_vals, shuffle=True)


if __name__ == "__main__":
    generator = TestDataGenerator()
    generator.generate_test_data()
