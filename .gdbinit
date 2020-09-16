# Update GDB's Python paths with the `sys.path` values of the local
#  Python installation, whether that is brew'ed Python, a virtualenv,
#  or another system python.

# Convert GDB to interpret in Python
python
import os,subprocess,sys
# Execute a Python using the user's shell and pull out the sys.path (for site-packages)
paths = subprocess.check_output('python3 -c "import os,sys;print(os.linesep.join(sys.path).strip())"',shell=True).decode("utf-8").split()
# Extend GDB's Python's search path
sys.path.extend(paths)
end

source tools/gdb.py
svd_load tools/STM32F103xx.svd
svd_load tools/ARMCM3.svd
