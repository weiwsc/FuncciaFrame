function(vva_configure_target target)
    target_compile_features(${target} PUBLIC cxx_std_23)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive-)
    else()
        target_compile_options(${target} PRIVATE
                -Wall -Wextra -Wpedantic -Wshadow
                -Werror=return-type
                -Werror=tautological-undefined-compare)
    endif()
endfunction()