![clc.png](https://github.com/user-attachments/assets/9fb320a7-2645-4089-9153-149f44e5720c)
# clc.
Super simple timer for debian.

you can find how to report bug for clc [here](https://github.com/Gfaerny/clc./blob/main/bug_report.md)

![build](https://github.com/Gfaerny/clc./actions/workflows/build.yml/badge.svg)
# how to build

clc debian package is in under proocess
so before build we have to install all clc build dependency

```bash
# installing all libraries and package that clc needs
chmod +x install_dependency.sh
./install_dependency.sh

# build
cmake .
make

```

# how to use
- keybind 
  - space : start/stop timer
  - r : restart record
  - q : quit app (if you use another method for closing clc it's possible clc wouldn't save time)
  
# at end
thanks for you attention. this project is super experimental so please feel free to report any typo , bug ... or any problem that you see
