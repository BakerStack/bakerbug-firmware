#!/usr/bin/env python3
import os
import sys

here = os.path.dirname(os.path.abspath(__file__))
common = os.path.join(here, "..", "esp_common", "esp_common_ident.py")

cmd = ["python3", common] + sys.argv[1:]
os.execv("/usr/bin/env", ["env"] + cmd)
