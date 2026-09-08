# Vendored MiniLibX

Source: https://github.com/dannywillems/minilibx-mac-osx

Upstream revision: `bae208e5502e200baf4b9e8188b7751e72ce5590`.

This directory is included as ordinary source files so cloning cub3D also
includes its graphics dependency. The upstream license is preserved in `LICENSE`.

Local change: `mlx_new_window.m` assigns the newly created OpenGL context to
the view with `[self setOpenGLContext:ctx]`, keeping initialization and rendering
on the same context.

Build artifacts are ignored and rebuilt locally by the project Makefile.
