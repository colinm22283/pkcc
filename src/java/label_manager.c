#include <java/label_manager.h>

#include <alloc.h>

void java_label_manager_init(java_label_manager_t * jlm) {
    jlm->current_label = 0;

    jlm->size = 0;
    jlm->capacity = 1;

    jlm->labels = pkcc_alloc(jlm->capacity * sizeof(java_label_t *));
}

void java_label_manager_free(java_label_manager_t * jlm) {
    for (size_t i = 0; i < jlm->size; i++) {
        pkcc_free(jlm->labels[i]->name);
        pkcc_free(jlm->labels[i]);
    }

    pkcc_free(jlm->labels);
}

java_label_t * java_label_manager_add(java_label_manager_t * jlm) {
    java_label_t * new_label = pkcc_alloc(sizeof(java_label_t));
    new_label->name = pkcc_alloc(15);
    sprintf(new_label->name, "L%zu", jlm->current_label++);

    jlm->labels[jlm->size++] = new_label;

    if (jlm->size == jlm->capacity) {
        jlm->capacity *= 2;

        jlm->labels = pkcc_realloc(jlm->labels, jlm->capacity * sizeof(java_label_t *));
    }

    return new_label;
}