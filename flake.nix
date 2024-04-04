{
  description = "Custom-Built MLIR";

  # Nixpkgs / NixOS version to use.
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachDefaultSystem (
      system: let
        # to work with older version of flakes
        lastModifiedDate = self.lastModifiedDate or self.lastModified or "19700101";
        # Generate a user-friendly version number.
        version = builtins.substring 0 8 lastModifiedDate;
        pkgs = import nixpkgs {inherit system;};
      in {
        packages.default = pkgs.llvmPackages_latest.stdenv.mkDerivation {
          name = "mlir-${version}";

          src = builtins.path {
            path = ./.;
            name = "source";
          };
          sourceRoot = "source/llvm";

          nativeBuildInputs = with pkgs; [
            python3
            ninja
            ccache
            cmake
            ncurses
            zlib
            llvmPackages_latest.llvm
            llvmPackages_latest.clang
            llvmPackages_latest.bintools
          ];

          buildInputs = with pkgs; [
            libxml2
          ];

          HOME = "~"; # For ccache to find the cache directory.

          doCheck = false;
          checkTarget = "check-mlir";

          cmakeFlags = [
            "-DCMAKE_BUILD_TYPE=Release"
            "-DLLVM_LINK_LLVM_DYLIB=ON"
            "-DLLVM_INSTALL_UTILS=ON"
            "-DLLVM_ENABLE_PROJECTS=mlir"
            "-DLLVM_BUILD_EXAMPLES=ON"
            "-DLLVM_TARGETS_TO_BUILD=X86;NVPTX;AMDGPU"
            "-DLLVM_ENABLE_ASSERTIONS=ON"
            "-DCMAKE_C_COMPILER=clang"
            "-DCMAKE_CXX_COMPILER=clang++"
            "-DLLVM_ENABLE_LLD=ON"
            "-DLLVM_CCACHE_BUILD=ON"
            # libxml2 needs to be disabled because the LLVM build system ignores its .la
            # file and doesn't link zlib as well.
            # https://github.com/ClangBuiltLinux/tc-build/issues/150#issuecomment-845418812
            "-DLLVM_ENABLE_LIBXML2=OFF"
          ];
        };

        devShells.default = pkgs.mkShell.override {stdenv = pkgs.llvmPackages_latest.stdenv;} {
          name = "mlir-dev";

          nativeBuildInputs = with pkgs; [
            python3
            ninja
            ccache
            cmake
            ncurses
            zlib
            llvmPackages_latest.llvm
            llvmPackages_latest.clang
            llvmPackages_latest.bintools
          ];

          buildInputs = with pkgs; [
            libxml2
          ];

          cmakeFlags = [
            "-GNinja"
            "-DCMAKE_BUILD_TYPE=Release"
            "-DLLVM_LINK_LLVM_DYLIB=ON"
            "-DLLVM_INSTALL_UTILS=ON"
            "-DLLVM_ENABLE_PROJECTS=mlir"
            "-DLLVM_BUILD_EXAMPLES=ON"
            "-DLLVM_TARGETS_TO_BUILD=X86;NVPTX;AMDGPU"
            "-DLLVM_ENABLE_ASSERTIONS=ON"
            "-DCMAKE_C_COMPILER=clang"
            "-DCMAKE_CXX_COMPILER=clang++"
            "-DLLVM_ENABLE_LLD=ON"
            "-DLLVM_CCACHE_BUILD=ON"
            # libxml2 needs to be disabled because the LLVM build system ignores its .la
            # file and doesn't link zlib as well.
            # https://github.com/ClangBuiltLinux/tc-build/issues/150#issuecomment-845418812
            "-DLLVM_ENABLE_LIBXML2=OFF"
          ];

          packages = with pkgs; [
            clang-tools
          ];
        };
      }
    );
}
