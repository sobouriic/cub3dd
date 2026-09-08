"""Exercise the real bonus parser and headless renderer with map fixtures."""
import os
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[1]
header = ("NO texture/NO.xpm\nSO texture/SO.xpm\n"
          "WE texture/WE.xpm\nEA texture/EA.xpm\n"
          "C 146,207,240\nF 103,147,68\n\n")
fixtures = [
    ("valid_newline", "11111\n1E001\n10001\n11111\n", True),
    ("valid_no_newline", "11111\n1E001\n10001\n11111", True),
    ("open_edge", "11111\n1E000\n10001\n11111\n", False),
    ("npc_at_edge", "11111\n1E00P\n10001\n11111\n", False),
    ("door_at_edge", "11111\n1E00D\n10001\n11111\n", False),
    ("unsupported_door", "1111111\n1E00001\n100D001\n1000001\n1111111\n", False),
    ("horizontal_door", "1111111\n1E00001\n111D111\n100P001\n1111111\n", True),
    ("ragged_open", "1111111\n1E00001\n100\n1111111\n", False),
    ("duplicate_player", "11111\n1E0N1\n11111\n", False),
]
with tempfile.TemporaryDirectory(prefix="blockyard-maps-") as directory:
    for name, grid, valid in fixtures:
        path = Path(directory) / f"{name}.cub"
        path.write_text(header + grid)
        env = dict(os.environ, CUB_CAPTURE=str(Path(directory) / "frame.ppm"))
        result = subprocess.run([str(root / "cub3D_bonus"), str(path)],
                                cwd=root, env=env, capture_output=True, timeout=10)
        assert (result.returncode == 0) == valid, (name, result.stdout, result.stderr)
        assert result.returncode >= 0, (name, "crashed", result.returncode)
        print("PASS:", name)

# The authored adventure must be completable, with the river separating its goals.
from collections import deque
lines = (root / "maps/island.cub").read_text().splitlines()
grid = lines[next(i for i, line in enumerate(lines) if line.startswith("111111")):]
spawn = next((x, y) for y, row in enumerate(grid) for x, tile in enumerate(row) if tile in "NSEW")
ores = [(x, y) for y, row in enumerate(grid) for x, tile in enumerate(row) if tile == "4"]
assert len(ores) == 6

def reachable(bridged):
    seen = {spawn}
    todo = deque([spawn])
    while todo:
        x, y = todo.popleft()
        for nx, ny in ((x-1,y),(x+1,y),(x,y-1),(x,y+1)):
            if not (0 <= ny < len(grid) and 0 <= nx < len(grid[ny])):
                continue
            walk = grid[ny][nx] in "0NSEWPDb4"
            walk |= bridged and ny == 15 and 23 <= nx <= 25
            if walk and (nx,ny) not in seen:
                seen.add((nx,ny))
                todo.append((nx,ny))
    return seen

assert sum(ore in reachable(False) for ore in ores) == 3
visited = reachable(True)
assert all(ore in visited for ore in ores)
portal = next((x,y) for y,row in enumerate(grid) for x,tile in enumerate(row) if tile == "T")
assert any((portal[0]+dx,portal[1]+dy) in visited for dx,dy in ((1,0),(-1,0),(0,1),(0,-1)))
print("PASS: island has six reachable crystals and a bridge-dependent beacon route")
