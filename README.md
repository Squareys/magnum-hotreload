Magnum/Corrade Hot Reload
=========================

*Proof of concept implementation of hot reloading using corrade.*

Inspired by
[a blog post on ourmachinery.com](http://ourmachinery.com/post/dll-hot-reloading-in-theory-and-practice/),
 I was motivated to finally implement hot reloading. As I use
[magnum](https://magnum.graphics) and [corrade](https://github.com/mosra/corrade/)
 for my lower-level projects, it is based on these (i.e. Corrade only, as it has
everything I need) and I hope the community will profit from that in some way.

Currently only Windows is supported

# Building

The project relies on [CMake](https://cmake.org/). You can use [vcpkg](https://github.com/Microsoft/vcpkg#quick-start)
or [build Corrade yourself](http://doc.magnum.graphics/corrade/building-corrade.html).
After that, building this project should be a matter of:

~~~
mkdir build && cd build
cmake ..
cmake --build . --target install
~~~

Potentially with additional parameters like `CMAKE_INSTALL_PREFIX` or
`CMAKE_TOOLCHAIN_FILE` depending on your specific setup.

# Running

After building, running on Windows is as simple as:

~~~
cd build/src
HotReloadApplication.exe
cd ..
# Make change in TestModule.cpp
cmake --build . --target TestModule
# Magic!
~~~

# Licence

The code of this project is licensed under the MIT/Expat license:

~~~
Copyright © 2018 Jonathan Hale <squareys@googlemail.com>

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
~~~
