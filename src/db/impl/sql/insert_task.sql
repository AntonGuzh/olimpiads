INSERT INTO task (
    id,
    olimpiad,
    year,
    tour,
    grades,
    grades_nubmers,
    topics,
    author,
    title,
    task_statement,
    task_solution,
    is_published
) VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, false);
