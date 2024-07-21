INSERT INTO img (
    task_id,
    size,
    width,
    heigth,
    name,
    content,
    is_statement,
    is_solution,
) VALUES ($1, $2, $3, $4,  $5, $6, $7, $8);
