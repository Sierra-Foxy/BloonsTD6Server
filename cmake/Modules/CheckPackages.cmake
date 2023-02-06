find_package(Boost ${minBoostVersion} REQUIRED
             COMPONENTS iostreams json)
find_package(ZLIB ${minZlibVersion} REQUIRED)
find_package(OpenSSL ${minOpenSSLVersion} REQUIRED) # also gets libcrypto