free_list_t free_list;

int main(int argc, char argv) {
    free_list_init(&free_list);

    options_parse_cli(argc, argv);

    rule_registry_init();

    file_loader_t file_loader;
    file_loader_init(&file_loader, options.input_path);
    free_list_node_t file_loader_node = free_list_push(&free_list, &file_loader, file_loader_free);
}

void exit_and_free(int exit_code) {
    free_list_free(&free_list);

    options_free();

    exit(exit_code);
}