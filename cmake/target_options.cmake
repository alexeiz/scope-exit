function(apply_project_options target scope)
    target_compile_options(${target} ${scope}
        $<$<CXX_COMPILER_ID:GNU>:-Wall>
        $<$<CXX_COMPILER_ID:GNU>:-Wextra>
        $<$<CXX_COMPILER_ID:GNU>:-Werror>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-implicit-fallthrough>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-unused-parameter>
        $<$<CXX_COMPILER_ID:GNU>:-Wunused-variable>
        $<$<CXX_COMPILER_ID:GNU>:-Wno-missing-field-initializers>
        $<$<CXX_COMPILER_ID:GNU>:-Wduplicated-cond>
        $<$<CXX_COMPILER_ID:GNU>:-Wlogical-op>
        $<$<CXX_COMPILER_ID:GNU>:-Wrestrict>
        $<$<CXX_COMPILER_ID:GNU>:-Wnull-dereference>
        $<$<CXX_COMPILER_ID:GNU>:-ffast-math>
        $<$<CXX_COMPILER_ID:GNU>:-finput-charset=UTF-8>
        $<$<CXX_COMPILER_ID:GNU>:-fexec-charset=UTF-8>
    )
endfunction()
