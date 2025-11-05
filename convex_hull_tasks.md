# Convex Hull Project - Task Breakdown & Assignments

## Team Size: 3 People
**Project Duration:** ~3-4 weeks (adjust based on your deadline)

---

## Phase 0: Initial Setup & Planning (Week 1 - Days 1-2)
**EVERYONE TOGETHER** - 4-6 hours

### Tasks:
1. **Interface Design Session** (2-3 hours)
   - Define common data structures for points and convex hulls
   - Agree on function signatures for each algorithm
   - Define input/output formats (file formats, data structures)
   - Agree on language choice (or stay language-agnostic initially)
   - Design module structure and file organization

2. **Performance Testing Strategy** (1 hour)
   - Function to test output correctness
   - Decide random test but deterministic generation approach (generate with a python script + writing on files)
      - first row is number of points n
      - next n rows are points (x y) in the format "x y\n"
   - Plan test case generation (square, circle, parabola Y=-X²)
   - Discuss additional test cases (if any)
   - Add shuffle points


3. **Scripting**
   - set up cmake for the build
   - set up just to run
   - set up python script to generate graphs to link with tyst

### Deliverables:
- [x] Interface specification document (markdown)
- [x] Initial project timeline
- [ ] Repository structure
   - [ ] Cmake and just file
   - [ ] Python scripts for test case generation and performance graphing

### Timeline

Sunday 9: complete Algorithms

Tuesday 11: complete tests and performance measurement correctness

Sunday 16: add performance measurements and graphs to report
