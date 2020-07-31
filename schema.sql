/*schema here*/

/*make sure its 3NF, go over sulis slides and your project and videos to make sure it is */


/*may not be in 3NF, see comment in issue #20 on GitHub*/
CREATE TABLE IF NOT EXISTS lecturers (
	email VARCHAR(100),
	name VARCHAR(100) NOT NULL,
	age INTEGER(3),
	department VARCHAR(100),
	PRIMARY KEY (email)
);

CREATE TABLE IF NOT EXISTS courses (
	id CHAR(5),
	type CHAR(5) NOT NULL, /*e.g BSc*/
	name VARCHAR(100) NOT NULL,
	duration INTEGER(1),
	course_leader VARCHAR(100),
	PRIMARY KEY (id),
	FOREIGN KEY (course_leader) REFERENCES lecturers(email)
		ON DELETE RESTRICT
		ON UPDATE CASCADE
);


/*may not be in 3NF, see comment in issue #20 on GitHub*/
CREATE TABLE IF NOT EXISTS students (
	id INTEGER NOT NULL,
	name VARCHAR(100) NOT NULL,
	age INTEGER,
	qca FLOAT DEFAULT 0.00, /*Maybe have a procedure or trigger that updates qca after grade is totalled */
	course CHAR(5),
	PRIMARY KEY (id),
	FOREIGN KEY (course) REFERENCES courses(id)
		ON DELETE RESTRICT
		ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS modules (
	code CHAR(6) NOT NULL,
	name VARCHAR(100),
	credits INTEGER(1),
	lecturer VARCHAR(100),
	PRIMARY KEY (code),
	FOREIGN KEY (lecturer) REFERENCES lecturers(email)
		ON DELETE RESTRICT
		ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS announcements (
	id INTEGER,
	module CHAR(6),
	lecturer VARCHAR(100),
	subject VARCHAR(250),
	announcement TEXT,
	time_created DATETIME DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY (id, module),
	FOREIGN KEY (module) REFERENCES modules(code)
		ON DELETE CASCADE 
		ON UPDATE CASCADE,
	FOREIGN KEY (lecturer) REFERENCES lecturers(email)
		ON DELETE CASCADE
		ON UPDATE CASCADE  
);

/*this is only a draft table*/
CREATE TABLE IF NOT EXISTS student_registrations (
	student INTEGER,
	module CHAR(6),
	PRIMARY KEY(student, module),
	FOREIGN KEY (student) REFERENCES students(id)
		ON DELETE CASCADE
		ON UPDATE CASCADE,
	FOREIGN KEY (module) REFERENCES modules(code)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS exams (
	id INTEGER NOT NULL,
	module CHAR(6),
	name VARCHAR(100),
	year INTEGER,
	semester INTEGER,
	numberOfQuestions INTEGER,
	weightPerQuestion FLOAT,
	totalWeight FLOAT,
	PRIMARY KEY (id, name),
	FOREIGN KEY (module) REFERENCES modules(code)
		ON DELETE CASCADE
		ON UPDATE CASCADE 
);

CREATE TABLE IF NOT EXISTS exam_questions (
	num INTEGER,
	exam INTEGER NOT NULL,
	question VARCHAR(100),
	answer_key VARCHAR(50),
	numberOfAnswers INTEGER,
	PRIMARY KEY (num, exam),
	FOREIGN KEY (exam) REFERENCES exams(id)
		ON DELETE CASCADE 
		ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS exam_answers (
	exam INTEGER,
	question INTEGER,
	answer VARCHAR(100),
	PRIMARY KEY (exam, question, answer),
	FOREIGN KEY (exam, question) REFERENCES exam_questions(exam, num)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS exam_grades (
	student INTEGER,
	exam INTEGER,
	grade FLOAT,
	date_recorded DATETIME DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY (student, exam),
	FOREIGN KEY (student) REFERENCES students(id)
		ON DELETE CASCADE
		ON UPDATE CASCADE,
	FOREIGN KEY (exam) REFERENCES exams(id)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS module_grades (
	module CHAR(6),
	student INTEGER,
	mark FLOAT,
	date_recorded DATETIME DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY (module, student),
	FOREIGN KEY (student) REFERENCES students(id)
		ON DELETE CASCADE 
		ON UPDATE CASCADE,
	FOREIGN KEY (module) REFERENCES modules(code)
		ON DELETE RESTRICT
		ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS lecturer_accounts (
	email VARCHAR(100),
	pass VARCHAR(16),
	PRIMARY KEY (email),
	FOREIGN KEY (email) REFERENCES lecturers(email)
		ON DELETE CASCADE 
		ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS student_accounts (
	id INTEGER,
	pass VARCHAR(16),
	PRIMARY KEY (id),
	FOREIGN KEY (id) REFERENCES students(id)
		ON DELETE CASCADE 
		ON UPDATE CASCADE 
);

CREATE TABLE IF NOT EXISTS lecturer_logins (
	email VARCHAR(100),
	loginTime DATETIME DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY (email, loginTime),
	FOREIGN KEY (email) REFERENCES lecturer_accounts(email)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS student_logins (
	id INTEGER,
	loginTime DATETIME DEFAULT CURRENT_TIMESTAMP,
	PRIMARY KEY (id, loginTime),
	FOREIGN KEY (id) REFERENCES student_accounts(id)
		ON DELETE CASCADE
		ON UPDATE CASCADE
);

/*other tables below*/
