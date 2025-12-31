#!/usr/bin/env python3
from pathlib import Path

HEADER = """// SPDX-License-Identifier: Apache-2.0
// Copyright (c) 2025 Daniel Flemström <daniel@flemstrom.nu>

"""

# Only apply to our own source trees
ROOTS = [
    Path("firmware/src"),
    Path("firmware/lib"),
    Path("tools"),
]

# Never touch these paths even if nested
EXCLUDE = [
    ".pio",
    "firmware/include/certs",
]

def is_excluded(path: Path) -> bool:
    s = str(path)
    return any(x in s for x in EXCLUDE)

def process_file(path: Path):
    if is_excluded(path):
        return False

    text = path.read_text(encoding="utf-8")

    if "SPDX-License-Identifier" in text:
        return False

    path.write_text(HEADER + text, encoding="utf-8")
    return True

count = 0

for root in ROOTS:
    if not root.exists():
        continue

    for p in root.rglob("*"):
        if p.suffix in (".cpp", ".h"):
            if process_file(p):
                print("Updated", p)
                count += 1

print(f"\n{count} files updated.")
