set(TARGET_NAME miceOS-tests)
add_executable(${TARGET_NAME}
    main.c
)
set_target_properties(${TARGET_NAME} PROPERTIES LANGUAGE C)
