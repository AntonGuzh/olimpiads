SELECT
    task.id AS id,
    task.olimpiad AS olimpiad,
    task.year AS year,
    task.tour AS tour,
    task.grades_nubmers AS grades_and_nubmers,
    task.topics AS topics,
    task.author AS author,
    task.title AS title,
    task.task_statement AS task_statement,
    task.task_solution AS task_solution,
    img.size AS img_size,
    width AS img_width,
    heigth AS img_height,
    name AS img_name,
    content AS img_content,
    is_statement AS is_statement_img,
    is_solution AS is_solution_img,
FROM task
WHERE
    ($1 IS NULL OR olimpiad IN (SELECT UNNEST($1))) AND
    ($2 IS NULL OR year >= $2) AND
    ($3 IS NULL OR year <= $3) AND
    ($4 IS NULL OR tour = $4) AND
    ($5 IS NULL OR grades && $5) AND
    ($6 IS NULL OR topics @> $6) AND
    ($7 IS NULL OR NOT topics && $7) AND
    id > $8 AND
    -- is_published TODO uncomment it after logging system
LEFT JOIN img ON task.id = img.task_id
ORDER BY (id)
LIMIT $9;