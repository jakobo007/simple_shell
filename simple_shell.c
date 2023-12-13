int main() {
    char *input = NULL;
    size_t size_of_input = 0;

    // Set up signal handlers
    signal(SIGTERM, handle_eof);
    signal(SIGINT, display_prompt);

    while (1) {
        display_prompt();

        if (getline(&input, &size_of_input, stdin) == -1) {
            perror("getline");
            exit(EXIT_FAILURE);
        }

        input[strcspn(input, "\n")] = '\0';
        tokenize_and_execute(input);
    }

    free(input);

    return 0;
}