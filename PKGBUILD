# Author: mosra <mosra@centrum.cz>
pkgname=kompas-core
pkgver=dev
pkgrel=1
pkgdesc="Core library for Kompas navigation system"
arch=('i686' 'x86_64')
url="http://mosra.cz/blog/kompas.php"
license=('LGPLv3')
makedepends=('cmake')
optdepends=('qt')
options=(!strip)

build() {
    mkdir -p "$startdir/build"
    cd "$startdir/build/"

    cmake .. \
        -DCMAKE_BUILD_TYPE=Debug \
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DBUILD_TESTS=TRUE
    make
}

package() {
  cd "$startdir/build"
  make DESTDIR="$pkgdir/" install
}
