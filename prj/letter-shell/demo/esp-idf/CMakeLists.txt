idf_component_register(
    SRCS "src/shell.c"
         "src/shell_ext.c"
         "shell_port.c"
    INCLUDE_DIRS "./"
                 "./src"
    LDFRAGMENTS "shell.lf"
)