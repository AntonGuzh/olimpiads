CREATE TABLE IF NOT EXISTS task (
    id text,
    olimpiad text NOT NULL,
    year integer NOT NULL,
    tour text NOT NULL,
    grades integer[] NOT NULL,
    grades_nubmers text[] NOT NULL,
    topics text[] NOT NULL,
    author text,
    title text,
    task_statement text,
    task_solution text,
    is_published boolean NOT NULL,
    PRIMARY KEY (id)
);

CREATE INDEX idx_topics ON task USING GIN(topics);
CREATE INDEX idx_grades ON task USING GIN(grades);
CREATE INDEX idx_olimpiad ON task USING btree(olimpiad, year, tour)

CREATE TABLE IF NOT EXISTS task_hash (
    hash integer,
    task_id text,
    PRIMARY KEY (hash),
    FOREIGN KEY (task_id) REFERENCES task (id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS img (
    task_id text NOT NULL,
    size integer NOT NULL,
    width integer NOT NULL,
    heigth integer NOT NULL,
    name text NOT NULL,
    content bytea NOT NULL,
    is_statement boolean NOT NULL,
    is_solution boolean NOT NULL,
    PRIMARY KEY (task_id, name),
    FOREIGN KEY (task_id) REFERENCES task (id) ON DELETE CASCADE
);
