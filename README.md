# screenarschlock
fake screelock app for linux. It's more like a screensaver for now but might be expanded to be smt more serious in the future.

# Build
```
git clone https://github.com/notfatnotfunny/screenarschlock.git
cd screenarschlock
touch rsc_path.txt
echo "$(pwd)" > rsc_path.txt
mkdir build
cd build
cmake ..
make
```
