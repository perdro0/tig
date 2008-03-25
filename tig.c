/* Copyright (c) 2006-2008 Jonas Fonseca <fonseca@diku.dk>
#define ID_COLS		8
#define NUMBER_INTERVAL	5
	"git log --no-color --topo-order --parents --boundary --pretty=raw %s 2>/dev/null"
#define TIG_BLAME_CMD	""
	unsigned int ltag:1;	/* If so, is the tag local? */
	unsigned int head:1;	/* Is it the current HEAD? */
	REQ_(VIEW_BLAME,	"Show blame view"), \
	REQ_(TOGGLE_DATE,	"Toggle date display"), \
	REQ_(TOGGLE_AUTHOR,	"Toggle author display"), \
	REQ_(TOGGLE_REFS,	"Toggle reference display (tags/branches)"), \
"   or: tig blame  [rev] path\n"
"  -h, --help      Show help message and exit";
static bool opt_date			= TRUE;
static bool opt_author			= TRUE;
static bool opt_show_refs		= TRUE;
static char opt_file[SIZEOF_STR]	= "";
static char opt_ref[SIZEOF_REF]		= "";
static char opt_head[SIZEOF_REF]	= "";
parse_options(int argc, char *argv[])
	size_t buf_size;
	char *subcommand;
	bool seen_dashdash = FALSE;
	int i;
	if (argc <= 1)
		return TRUE;
	subcommand = argv[1];
	if (!strcmp(subcommand, "status") || !strcmp(subcommand, "-S")) {
		opt_request = REQ_VIEW_STATUS;
		if (!strcmp(subcommand, "-S"))
			warn("`-S' has been deprecated; use `tig status' instead");
		if (argc > 2)
			warn("ignoring arguments after `%s'", subcommand);
		return TRUE;
	} else if (!strcmp(subcommand, "blame")) {
		opt_request = REQ_VIEW_BLAME;
		if (argc <= 2 || argc > 4)
			die("invalid number of options to blame\n\n%s", usage);
		i = 2;
		if (argc == 4) {
			string_ncopy(opt_ref, argv[i], strlen(argv[i]));
			i++;
		}
		string_ncopy(opt_file, argv[i], strlen(argv[i]));
		return TRUE;
	} else if (!strcmp(subcommand, "show")) {
		opt_request = REQ_VIEW_DIFF;
	} else if (!strcmp(subcommand, "log") || !strcmp(subcommand, "diff")) {
		opt_request = subcommand[0] == 'l'
			    ? REQ_VIEW_LOG : REQ_VIEW_DIFF;
		warn("`tig %s' has been deprecated", subcommand);

	} else {
		subcommand = NULL;
	if (!subcommand)
		/* XXX: This is vulnerable to the user overriding
		 * options required for the main view parser. */
		string_copy(opt_cmd, "git log --no-color --pretty=raw --boundary --parents");
	else
		string_format(opt_cmd, "git %s", subcommand);
	buf_size = strlen(opt_cmd);
	for (i = 1 + !!subcommand; i < argc; i++) {
		if (seen_dashdash || !strcmp(opt, "--")) {
			seen_dashdash = TRUE;
		} else if (!strcmp(opt, "-v") || !strcmp(opt, "--version")) {
		} else if (!strcmp(opt, "-h") || !strcmp(opt, "--help")) {
		opt_cmd[buf_size++] = ' ';
		buf_size = sq_quote(opt_cmd, buf_size, opt);
		if (buf_size >= sizeof(opt_cmd))
			die("command too long");
		buf_size = 0;
	opt_cmd[buf_size] = 0;
LINE(MAIN_LOCAL_TAG,"",			COLOR_MAGENTA,	COLOR_DEFAULT,	A_BOLD), \
LINE(MAIN_HEAD,    "",			COLOR_RED,	COLOR_DEFAULT,	A_BOLD), \
LINE(STAT_UNTRACKED,"",			COLOR_MAGENTA,	COLOR_DEFAULT,	0), \
LINE(BLAME_DATE,    "",			COLOR_BLUE,	COLOR_DEFAULT,	0), \
LINE(BLAME_AUTHOR,  "",			COLOR_GREEN,	COLOR_DEFAULT,	0), \
LINE(BLAME_COMMIT, "",			COLOR_DEFAULT,	COLOR_DEFAULT,	0), \
LINE(BLAME_ID,     "",			COLOR_MAGENTA,	COLOR_DEFAULT,	0), \
LINE(BLAME_LINENO, "",			COLOR_CYAN,	COLOR_DEFAULT,	0)
	unsigned int dirty:1;
	{ 'B',		REQ_VIEW_BLAME },
	{ 'D',		REQ_TOGGLE_DATE },
	{ 'A',		REQ_TOGGLE_AUTHOR },
	{ 'F',		REQ_TOGGLE_REFS },
	KEYMAP_(BLAME), \
static bool parse_bool(const char *s)
{
	return (!strcmp(s, "1") || !strcmp(s, "true") ||
		!strcmp(s, "yes")) ? TRUE : FALSE;
}

	if (!strcmp(argv[0], "show-author")) {
		opt_author = parse_bool(argv[2]);
		return OK;
	}

	if (!strcmp(argv[0], "show-date")) {
		opt_date = parse_bool(argv[2]);
		return OK;
	}

		opt_rev_graph = parse_bool(argv[2]);
		return OK;
	}

	if (!strcmp(argv[0], "show-refs")) {
		opt_show_refs = parse_bool(argv[2]);
		return OK;
	}

	if (!strcmp(argv[0], "show-line-numbers")) {
		opt_line_number = parse_bool(argv[2]);
	size_t lines;		/* Total number of lines */
	size_t line_alloc;	/* Total number of allocated lines */
	size_t line_size;	/* Total number of used lines */
static struct view_ops blame_ops;
	VIEW_(BLAME,  "blame",  &blame_ops,  ref_commit),
draw_text(struct view *view, const char *string, int max_len,
static void
redraw_view_dirty(struct view *view)
{
	bool dirty = FALSE;
	int lineno;

	for (lineno = 0; lineno < view->height; lineno++) {
		struct line *line = &view->line[view->offset + lineno];

		if (!line->dirty)
			continue;
		line->dirty = 0;
		dirty = TRUE;
		if (!draw_view_line(view, lineno))
			break;
	}

	if (!dirty)
		return;
	redrawwin(view->win);
	if (input_mode)
		wnoutrefresh(view->win);
	else
		wrefresh(view->win);
}

#define ITEM_CHUNK_SIZE 256
static void *
realloc_items(void *mem, size_t *size, size_t new_size, size_t item_size)
{
	size_t num_chunks = *size / ITEM_CHUNK_SIZE;
	size_t num_chunks_new = (new_size + ITEM_CHUNK_SIZE - 1) / ITEM_CHUNK_SIZE;

	if (mem == NULL || num_chunks != num_chunks_new) {
		*size = num_chunks_new * ITEM_CHUNK_SIZE;
		mem = realloc(mem, *size * item_size);
	}

	return mem;
}

	size_t alloc = view->line_alloc;
	struct line *tmp = realloc_items(view->line, &alloc, line_size,
					 sizeof(*view->line));
	view->line_alloc = alloc;

	if (view == VIEW(REQ_VIEW_BLAME))
		redraw_view_dirty(view);

	if (view->ops->read(view, NULL))
		end_update(view);
	case REQ_VIEW_BLAME:
		if (!opt_file[0]) {
			report("No file chosen, press %s to open tree view",
			       get_key(REQ_VIEW_TREE));
			break;
		}
		open_view(view, request, OPEN_DEFAULT);
		break;

		   (view == VIEW(REQ_VIEW_DIFF) &&
		     view->parent == VIEW(REQ_VIEW_BLAME)) ||
	case REQ_TOGGLE_DATE:
		opt_date = !opt_date;
		redraw_display();
		break;

	case REQ_TOGGLE_AUTHOR:
		opt_author = !opt_author;
		redraw_display();
		break;

	case REQ_TOGGLE_REFS:
		opt_show_refs = !opt_show_refs;
		redraw_display();
		break;

		draw_text(view, text, view->width, TRUE, tilde_attr);
static char *
tree_path(struct line *line)
{
	char *path = line->data;

	return path + SIZEOF_TREE_ATTR;
}

	if (!text)
		return TRUE;
		char *path1 = tree_path(line);
	if (request == REQ_VIEW_BLAME) {
		char *filename = tree_path(line);

		if (line->type == LINE_TREE_DIR) {
			report("Cannot show blame for directory %s", opt_path);
			return REQ_NONE;
		}

		string_copy(opt_ref, view->vid);
		string_format(opt_file, "%s%s", opt_path, filename);
		return request;
	}
			char *basename = tree_path(line);
	if (!line)
		return TRUE;
/*
 * Blame backend
 *
 * Loading the blame view is a two phase job:
 *
 *  1. File content is read either using opt_file from the
 *     filesystem or using git-cat-file.
 *  2. Then blame information is incrementally added by
 *     reading output from git-blame.
 */

struct blame_commit {
	char id[SIZEOF_REV];		/* SHA1 ID. */
	char title[128];		/* First line of the commit message. */
	char author[75];		/* Author of the commit. */
	struct tm time;			/* Date from the author ident. */
	char filename[128];		/* Name of file. */
};

struct blame {
	struct blame_commit *commit;
	unsigned int header:1;
	char text[1];
};

#define BLAME_CAT_FILE_CMD "git cat-file blob %s:%s"
#define BLAME_INCREMENTAL_CMD "git blame --incremental %s %s"

static bool
blame_open(struct view *view)
{
	char path[SIZEOF_STR];
	char ref[SIZEOF_STR] = "";

	if (sq_quote(path, 0, opt_file) >= sizeof(path))
		return FALSE;

	if (*opt_ref && sq_quote(ref, 0, opt_ref) >= sizeof(ref))
		return FALSE;

	if (*opt_ref) {
		if (!string_format(view->cmd, BLAME_CAT_FILE_CMD, ref, path))
			return FALSE;
	} else {
		view->pipe = fopen(opt_file, "r");
		if (!view->pipe &&
		    !string_format(view->cmd, BLAME_CAT_FILE_CMD, "HEAD", path))
			return FALSE;
	}

	if (!view->pipe)
		view->pipe = popen(view->cmd, "r");
	if (!view->pipe)
		return FALSE;

	if (!string_format(view->cmd, BLAME_INCREMENTAL_CMD, ref, path))
		return FALSE;

	string_format(view->ref, "%s ...", opt_file);
	string_copy_rev(view->vid, opt_file);
	set_nonblocking_input(TRUE);

	if (view->line) {
		int i;

		for (i = 0; i < view->lines; i++)
			free(view->line[i].data);
		free(view->line);
	}

	view->lines = view->line_alloc = view->line_size = view->lineno = 0;
	view->offset = view->lines  = view->lineno = 0;
	view->line = NULL;
	view->start_time = time(NULL);

	return TRUE;
}

static struct blame_commit *
get_blame_commit(struct view *view, const char *id)
{
	size_t i;

	for (i = 0; i < view->lines; i++) {
		struct blame *blame = view->line[i].data;

		if (!blame->commit)
			continue;

		if (!strncmp(blame->commit->id, id, SIZEOF_REV - 1))
			return blame->commit;
	}

	{
		struct blame_commit *commit = calloc(1, sizeof(*commit));

		if (commit)
			string_ncopy(commit->id, id, SIZEOF_REV);
		return commit;
	}
}

static bool
parse_number(char **posref, size_t *number, size_t min, size_t max)
{
	char *pos = *posref;

	*posref = NULL;
	pos = strchr(pos + 1, ' ');
	if (!pos || !isdigit(pos[1]))
		return FALSE;
	*number = atoi(pos + 1);
	if (*number < min || *number > max)
		return FALSE;

	*posref = pos;
	return TRUE;
}

static struct blame_commit *
parse_blame_commit(struct view *view, char *text, int *blamed)
{
	struct blame_commit *commit;
	struct blame *blame;
	char *pos = text + SIZEOF_REV - 1;
	size_t lineno;
	size_t group;

	if (strlen(text) <= SIZEOF_REV || *pos != ' ')
		return NULL;

	if (!parse_number(&pos, &lineno, 1, view->lines) ||
	    !parse_number(&pos, &group, 1, view->lines - lineno + 1))
		return NULL;

	commit = get_blame_commit(view, text);
	if (!commit)
		return NULL;

	*blamed += group;
	while (group--) {
		struct line *line = &view->line[lineno + group - 1];

		blame = line->data;
		blame->commit = commit;
		line->dirty = 1;
	}
	blame->header = 1;

	return commit;
}

static bool
blame_read_file(struct view *view, char *line)
{
	if (!line) {
		FILE *pipe = NULL;

		if (view->lines > 0)
			pipe = popen(view->cmd, "r");
		view->cmd[0] = 0;
		if (!pipe) {
			report("Failed to load blame data");
			return TRUE;
		}

		fclose(view->pipe);
		view->pipe = pipe;
		return FALSE;

	} else {
		size_t linelen = strlen(line);
		struct blame *blame = malloc(sizeof(*blame) + linelen);

		if (!line)
			return FALSE;

		blame->commit = NULL;
		strncpy(blame->text, line, linelen);
		blame->text[linelen] = 0;
		return add_line_data(view, blame, LINE_BLAME_COMMIT) != NULL;
	}
}

static bool
match_blame_header(const char *name, char **line)
{
	size_t namelen = strlen(name);
	bool matched = !strncmp(name, *line, namelen);

	if (matched)
		*line += namelen;

	return matched;
}

static bool
blame_read(struct view *view, char *line)
{
	static struct blame_commit *commit = NULL;
	static int blamed = 0;
	static time_t author_time;

	if (*view->cmd)
		return blame_read_file(view, line);

	if (!line) {
		/* Reset all! */
		commit = NULL;
		blamed = 0;
		string_format(view->ref, "%s", view->vid);
		if (view_is_displayed(view)) {
			update_view_title(view);
			redraw_view_from(view, 0);
		}
		return TRUE;
	}

	if (!commit) {
		commit = parse_blame_commit(view, line, &blamed);
		string_format(view->ref, "%s %2d%%", view->vid,
			      blamed * 100 / view->lines);

	} else if (match_blame_header("author ", &line)) {
		string_ncopy(commit->author, line, strlen(line));

	} else if (match_blame_header("author-time ", &line)) {
		author_time = (time_t) atol(line);

	} else if (match_blame_header("author-tz ", &line)) {
		long tz;

		tz  = ('0' - line[1]) * 60 * 60 * 10;
		tz += ('0' - line[2]) * 60 * 60;
		tz += ('0' - line[3]) * 60;
		tz += ('0' - line[4]) * 60;

		if (line[0] == '-')
			tz = -tz;

		author_time -= tz;
		gmtime_r(&author_time, &commit->time);

	} else if (match_blame_header("summary ", &line)) {
		string_ncopy(commit->title, line, strlen(line));

	} else if (match_blame_header("filename ", &line)) {
		string_ncopy(commit->filename, line, strlen(line));
		commit = NULL;
	}

	return TRUE;
}

static bool
blame_draw(struct view *view, struct line *line, unsigned int lineno, bool selected)
{
	int tilde_attr = -1;
	struct blame *blame = line->data;
	int col = 0;

	wmove(view->win, lineno, 0);

	if (selected) {
		wattrset(view->win, get_line_attr(LINE_CURSOR));
		wchgat(view->win, -1, 0, LINE_CURSOR, NULL);
	} else {
		wattrset(view->win, A_NORMAL);
		tilde_attr = get_line_attr(LINE_MAIN_DELIM);
	}

	if (opt_date) {
		int n;

		if (!selected)
			wattrset(view->win, get_line_attr(LINE_MAIN_DATE));
		if (blame->commit) {
			char buf[DATE_COLS + 1];
			int timelen;

			timelen = strftime(buf, sizeof(buf), DATE_FORMAT, &blame->commit->time);
			n = draw_text(view, buf, view->width - col, FALSE, tilde_attr);
			draw_text(view, " ", view->width - col - n, FALSE, tilde_attr);
		}

		col += DATE_COLS;
		wmove(view->win, lineno, col);
		if (col >= view->width)
			return TRUE;
	}

	if (opt_author) {
		int max = MIN(AUTHOR_COLS - 1, view->width - col);

		if (!selected)
			wattrset(view->win, get_line_attr(LINE_MAIN_AUTHOR));
		if (blame->commit)
			draw_text(view, blame->commit->author, max, TRUE, tilde_attr);
		col += AUTHOR_COLS;
		if (col >= view->width)
			return TRUE;
		wmove(view->win, lineno, col);
	}

	{
		int max = MIN(ID_COLS - 1, view->width - col);

		if (!selected)
			wattrset(view->win, get_line_attr(LINE_BLAME_ID));
		if (blame->commit)
			draw_text(view, blame->commit->id, max, FALSE, -1);
		col += ID_COLS;
		if (col >= view->width)
			return TRUE;
		wmove(view->win, lineno, col);
	}

	{
		unsigned long real_lineno = view->offset + lineno + 1;
		char number[10] = "          ";
		int max = MIN(view->digits, STRING_SIZE(number));
		bool showtrimmed = FALSE;

		if (real_lineno == 1 ||
		    (real_lineno % opt_num_interval) == 0) {
			char fmt[] = "%1ld";

			if (view->digits <= 9)
				fmt[1] = '0' + view->digits;

			if (!string_format(number, fmt, real_lineno))
				number[0] = 0;
			showtrimmed = TRUE;
		}

		if (max > view->width - col)
			max = view->width - col;
		if (!selected)
			wattrset(view->win, get_line_attr(LINE_BLAME_LINENO));
		col += draw_text(view, number, max, showtrimmed, tilde_attr);
		if (col >= view->width)
			return TRUE;
	}

	if (!selected)
		wattrset(view->win, A_NORMAL);

	if (col >= view->width)
		return TRUE;
	waddch(view->win, ACS_VLINE);
	col++;
	if (col >= view->width)
		return TRUE;
	waddch(view->win, ' ');
	col++;
	col += draw_text(view, blame->text, view->width - col, TRUE, tilde_attr);

	return TRUE;
}

static enum request
blame_request(struct view *view, enum request request, struct line *line)
{
	enum open_flags flags = display[0] == view ? OPEN_SPLIT : OPEN_DEFAULT;
	struct blame *blame = line->data;

	switch (request) {
	case REQ_ENTER:
		if (!blame->commit) {
			report("No commit loaded yet");
			break;
		}

		if (!strcmp(blame->commit->id, "0000000000000000000000000000000000000000")) {
			char path[SIZEOF_STR];

			if (sq_quote(path, 0, view->vid) >= sizeof(path))
				break;
			string_format(opt_cmd, "git diff-index --root --patch-with-stat -C -M --cached HEAD -- %s 2>/dev/null", path);
		}

		open_view(view, REQ_VIEW_DIFF, flags);
		break;

	default:
		return request;
	}

	return REQ_NONE;
}

static bool
blame_grep(struct view *view, struct line *line)
{
	struct blame *blame = line->data;
	struct blame_commit *commit = blame->commit;
	regmatch_t pmatch;

#define MATCH(text) \
	(*text && regexec(view->regex, text, 1, &pmatch, 0) != REG_NOMATCH)

	if (commit) {
		char buf[DATE_COLS + 1];

		if (MATCH(commit->title) ||
		    MATCH(commit->author) ||
		    MATCH(commit->id))
			return TRUE;

		if (strftime(buf, sizeof(buf), DATE_FORMAT, &commit->time) &&
		    MATCH(buf))
			return TRUE;
	}

	return MATCH(blame->text);

#undef MATCH
}

static void
blame_select(struct view *view, struct line *line)
{
	struct blame *blame = line->data;
	struct blame_commit *commit = blame->commit;

	if (!commit)
		return;

	if (!strcmp(commit->id, "0000000000000000000000000000000000000000"))
		string_ncopy(ref_commit, "HEAD", 4);
	else
		string_copy_rev(ref_commit, commit->id);
}

static struct view_ops blame_ops = {
	"line",
	blame_open,
	blame_read,
	blame_draw,
	blame_request,
	blame_grep,
	blame_select,
};
#define STATUS_DIFF_FILES_CMD "git diff-files -z"
	view->lines = view->line_alloc = view->line_size = view->lineno = 0;
	system("git update-index -q --refresh");

		draw_text(view, text, view->width, TRUE, tilde_attr);
	draw_text(view, status->new.name, view->width - 5, TRUE, tilde_attr);
	case REQ_VIEW_BLAME:
		if (status) {
			string_copy(opt_file, status->new.name);
			opt_ref[0] = 0;
		}
		return request;

	case REQ_VIEW_BLAME:
		if (stage_status.new.name[0]) {
			string_copy(opt_file, stage_status.new.name);
			opt_ref[0] = 0;
		}
		return request;

	bool has_parents;		/* Rewritten --parents seen. */
	if (opt_date) {
		n = draw_text(view, buf, view->width - col, FALSE, tilde_attr);
		draw_text(view, " ", view->width - col - n, FALSE, tilde_attr);
	if (opt_author) {
		draw_text(view, commit->author, max_len, TRUE, tilde_attr);
	if (opt_show_refs && commit->refs) {
			else if (commit->refs[i]->head)
				wattrset(view->win, get_line_attr(LINE_MAIN_HEAD));
			else if (commit->refs[i]->ltag)
				wattrset(view->win, get_line_attr(LINE_MAIN_LOCAL_TAG));
			col += draw_text(view, "[", view->width - col, TRUE, tilde_attr);
			col += draw_text(view, commit->refs[i]->name, view->width - col,
					 TRUE, tilde_attr);
			col += draw_text(view, "]", view->width - col, TRUE, tilde_attr);
			col += draw_text(view, " ", view->width - col, TRUE, tilde_attr);
	draw_text(view, commit->title, view->width - col, TRUE, tilde_attr);

		while ((line = strchr(line, ' '))) {
			line++;
			push_rev_graph(graph->parents, line);
			commit->has_parents = TRUE;
		}
		if (commit->has_parents)
			break;
static struct ref *refs = NULL;
static size_t refs_alloc = 0;
static size_t refs_size = 0;
static struct ref ***id_refs = NULL;
static size_t id_refs_alloc = 0;
static size_t id_refs_size = 0;
	size_t ref_list_alloc = 0;
	tmp_id_refs = realloc_items(id_refs, &id_refs_alloc, id_refs_size + 1,
				    sizeof(*id_refs));
		tmp = realloc_items(ref_list, &ref_list_alloc,
				    ref_list_size + 1, sizeof(*ref_list));
	bool ltag = FALSE;
	bool check_replace = FALSE;
	bool head = FALSE;
		if (!strcmp(name + namelen - 3, "^{}")) {
			namelen -= 3;
			name[namelen] = 0;
			if (refs_size > 0 && refs[refs_size - 1].ltag == TRUE)
				check_replace = TRUE;
		} else {
			ltag = TRUE;
		}
		head	 = !strncmp(opt_head, name, namelen);
	if (check_replace && !strcmp(name, refs[refs_size - 1].name)) {
		/* it's an annotated tag, replace the previous sha1 with the
		 * resolved commit id; relies on the fact git-ls-remote lists
		 * the commit id of an annotated tag right beofre the commit id
		 * it points to. */
		refs[refs_size - 1].ltag = ltag;
		string_copy_rev(refs[refs_size - 1].id, id);

		return OK;
	}
	refs = realloc_items(refs, &refs_alloc, refs_size + 1, sizeof(*refs));
	ref->ltag = ltag;
	ref->head = head;
	} else if (opt_cdup[0] == ' ') {
	} else {
		if (!strncmp(name, "refs/heads/", STRING_SIZE("refs/heads/"))) {
			namelen -= STRING_SIZE("refs/heads/");
			name	+= STRING_SIZE("refs/heads/");
			string_ncopy(opt_head, name, namelen);
		}
	int result;
	FILE *pipe = popen("git rev-parse --git-dir --is-inside-work-tree "
			   " --show-cdup --symbolic-full-name HEAD 2>/dev/null", "r");

	/* XXX: The line outputted by "--show-cdup" can be empty so
	 * initialize it to something invalid to make it possible to
	 * detect whether it has been set or not. */
	opt_cdup[0] = ' ';

	result = read_properties(pipe, "=", read_repo_info);
	if (opt_cdup[0] == ' ')
		opt_cdup[0] = 0;

	return result;
	if (*opt_encoding && strcasecmp(opt_encoding, "UTF-8"))
		opt_utf8 = FALSE;
