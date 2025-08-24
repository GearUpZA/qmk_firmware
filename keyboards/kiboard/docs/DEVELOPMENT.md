# Development Guide

## Project Structure
```
kiboard/
├── config.h       # Hardware configuration
├── kiboard.c      # Main keyboard logic
├── kiboard.h      # Main header file
├── config_ui.c    # UI implementation
├── config_ui.h    # UI definitions
├── drivers/       # Hardware drivers
│   ├── display/   # LCD driver
│   └── touch/     # Touch controller
└── docs/          # Documentation
```

## Adding New Features
1. Create feature branch
2. Update documentation
3. Add test cases
4. Submit PR

## Coding Standards
- Use descriptive variable names
- Comment complex logic
- Follow QMK coding conventions
