# Installation Engine

A console-based installation dependency manager written in C++.

This project simulates a small package/module installation system with support for:

- adding components
- attaching dependencies
- installing and uninstalling components
- mocking installation failures
- resolving failed components
- observing state changes through a logger

---

## Features

- Add components as `MODULE` or `PACKAGE`
- Attach components to packages as dependencies
- Install individual components with dependency handling
- Uninstall a single component or all installed components
- Simulate installation failure using mock-fail commands
- Resolve failed components and retry installation
- Track state changes using the Observer pattern
- Prevent invalid operations such as duplicate IDs or invalid attachments

---

## Component Types

### Module
A basic installable component with no children.

### Package
A composite installable component that can contain other modules or packages as dependencies.

---

## Component States

Each component can be in one of the following states:

- `PENDING` - not installed
- `INSTALLED` - successfully installed
- `FAILED` - installation failed

---

## Design Overview

This project uses an object-oriented design with a few key ideas:

### 1. Inheritance
A common abstract base class is used for all installable components.

- `installable` is the base class
- `module` and `package` derive from it

### 2. Composite-like Structure
A `package` can contain other components and install/uninstall them recursively.

### 3. Observer Pattern
The logger listens to component state changes and prints state transition messages automatically.

### 4. Transaction-style Installation
During installation, state changes and parent-count changes are tracked so the process can remain controlled and consistent.

├── observer.h
└── componentState.h
