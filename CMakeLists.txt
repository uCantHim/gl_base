cmake_minimum_required(VERSION 3.13)

# Global compiler settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
# set(CMAKE_CXX_CLANG_TIDY
#     clang-tidy;
#     -header-filter=.;
#     -checks=bugprone-*,cert-*,clang-analyzer-*,cppcoreguidelines-avoid-goto,cppcoreguidelines-init-variables,cppcoreguidelines-interfaces-global-init,cppcoreguidelines-narrowing-conversions,cppcoreguidelines-no-malloc,cppcoreguidelines-array-to-pointer-decay,cppcoreguidelines-pro-type-const-cast,cppcoreguidelines-pro-type-cstyle-cast,cppcoreguidelines-pro-type-member-init,cppcoreguidelines-pro-type-vararg,cppcoreguidelines-slicing,cppcoreguidelines-special-member-functions,modernize-avoid-bind,modernize-avoid-c-arrays,modernize-concat-nested-namespaces,modernize-deprecated-headers,modernize-deprecated-ios-base-aliases,modernize-loop-convert,modernize-make-shared,modernize-make-unique,modernize-pass-by-value,modernize-raw-string-literal,modernize-redundant-void-arg,modernize-replace-auto-ptr,modernize-replace-random-shuffle,modernize-return-braced-init-list,modernize-shrink-to-fit,modernize-unary-static-assert,modernize-use-auto,modernize-use-bool-literals,modernize-use-default-member-init,modernize-use-emplace,modernize-use-equals-default,modernize-use-equals-delete,modernize-use-nodiscard,modernize-use-noexcept,modernize-use-nullptr,modernize-use-override,modernize-use-transparent-functors,modernize-use-uncaught-exceptions,modernize-use-using,performance-*,portablility-*,readability-container-size-empty,readability-convert-member-functions-to-static,readability-delete-null-pointer,readbility-deleted-default,readability-else-after-return,readability-magic-numbers,readability-make-member-function-const,readability-redundant-member-init,readability-redundant-smartptr-get,readability-redundant-string-cstr,readability-redundant-string-init,readability-simplify-boolean-expr,misc-definitions-in-headers,misc-misplaced-const,misc-new-delete-overloads,misc-no-recursion,misc-non-copyable-objects,misc-redundant-expression,misc-static-assert,misc-throw-by-value-catch-by-reference,misc-unconventional-assign-operator,misc-uniqueptr-reset-release,misc-unused-alias-decls,misc-unused-parameters,misc-unused-using-decls
#     ;
# )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (UNIX AND NOT APPLE)
	set(LINUX TRUE)
endif()

# Project
project(opengl_base_utilities)

add_library(gl_base)
add_subdirectory(src)
