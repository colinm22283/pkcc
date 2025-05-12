#include <java/header_footer.h>

const char * java_out_file_header = ".class public %s\n"
                                    ".super java/lang/Object\n\n";

const char * java_out_file_footer = "\t.method public static main : ([Ljava/lang/String;)V\n"
                                    "\t.code stack 1 locals 1\n"
                                    "\t\tinvokestatic Method %s main ()I\n"
                                    "\t\tinvokestatic Method java/lang/System exit (I)V\n"
                                    "\t\treturn\n"
                                    "\t.end code\n"
                                    "\t.end method\n";

void java_print_header(FILE * out_file, const char * classname) {
    fprintf(out_file, java_out_file_header, classname);
}

void java_print_footer(FILE * out_file, const char * classname) {
    fprintf(out_file, java_out_file_footer, classname);
}