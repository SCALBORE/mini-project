# 🛠️ 2D Terminal Graphics Engine

A menu-driven, state-managed 2D graphics editor written entirely in C. It utilizes a custom Display List architecture to allow dynamic modification and deletion of geometric objects on an ASCII canvas directly in the terminal.

## 🌐 Live Project Dashboard
You can view the project presentation and documentation here: **[Live Dashboard](https://scalbore.github.io/mini-project/)**

---

## ✨ Features

* **Mathematical Rasterization:** Implements Bresenham's Line Algorithm and the Midpoint Circle Algorithm from scratch without relying on external graphics libraries.
* **State Management:** Uses a `struct`-based Display List to track Object IDs and parameters, allowing for non-destructive rendering and real-time object deletion/modification.
* **Dynamic Canvas:** Add, Delete, and Modify geometries (Lines, Rectangles, Circles, Triangles) via a menu-driven CLI.
* **Aesthetic Interface:** Utilizes standard ANSI escape sequences to map out a modern, color-coded terminal user interface.

---

## 📂 File Structure

* `editor.c` - The core C engine containing the math algorithms, display list logic, and terminal rendering loop.
* `index.html` - A responsive HTML/CSS dashboard used to present the project features and documentation.

---

## 🚀 Quick Start Guide

To run the graphics engine locally, you will need a C compiler (like GCC) installed on your system.

**1. Clone the repository:**
```bash
git clone [https://github.com/scalbore/mini-project.git](https://github.com/scalbore/mini-project.git)
cd mini-project
