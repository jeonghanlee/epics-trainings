#!/usr/bin/env python3
"""Generate the single-page printable ALS-U EPICS training quick reference PDF."""

from pathlib import Path

from reportlab.lib.colors import HexColor, white
from reportlab.lib.pagesizes import letter
from reportlab.lib.units import inch
from reportlab.pdfgen import canvas

REPO_ROOT = Path(__file__).resolve().parent.parent
OUTPUT_PATH = REPO_ROOT / "docs" / "assets" / "epics-training-quick-ref.pdf"

PAGE_W, PAGE_H = letter
MARGIN = 0.5 * inch
CONTENT_W = PAGE_W - 2 * MARGIN
SECTION_GAP = 15
TITLE_TO_TABLE_GAP = 8
CELL_PAD = 6

DARK_BG = HexColor("#2C3E50")
ACCENT = HexColor("#2980B9")
WARN_RED = HexColor("#C0392B")
OK_GREEN = HexColor("#27AE60")
LIGHT_BG = HexColor("#F5F7FA")
LIGHT_GRAY = HexColor("#BDC3C7")
GRID_GRAY = HexColor("#DEE2E6")
MUTED_GRAY = HexColor("#7F8C8D")
TABLE_HEADER_BG = HexColor("#34495E")
TABLE_ROW_ALT = HexColor("#ECF0F1")
PRINCIPLE_BG = HexColor("#EBF5FB")
WARN_BG = HexColor("#FDEDEC")

DOC_TITLE = "ALS-U EPICS Environment Quick Reference"
DOC_SUBTITLE = (
    "EPICS-env 1.5.0  |  EPICS Base 7.0.10  |  PVXS / QSRV2  |  "
    "Local Loopback Exercises"
)
DOC_REF = "ALS-U Controls Training  |  Ref: jeonghanlee.github.io/epics-trainings"
DOC_AUTHOR = "Author: Jeong Han Lee, Dr.rer.nat"
DOC_BADGE = "SINGLE-PAGE QUICK REFERENCE"

PRINCIPLE_HEADING = "Core Session and Loopback Rules"
PRINCIPLE_LINES = [
    '1. Activate one matching OS tree per shell:  source "$TREE/setEpicsEnv.bash"  (TREE=<dist>/1.5.0/<os-id>/7.0.10).',
    "2. Pin CA and PVA clients to local loopback (127.0.0.1) with AUTO_ADDR_LIST=NO in every client terminal.",
    "3. Run one active IOC per PV prefix and TCP port (9399-9401); stop periodic TCP emulators before exit at iocsh.",
]

ENV_TITLE = "1. Environment Activation and Loopback Discovery"
ENV_HEADERS = ["Setting / Check", "Shell Command", "Role in Exercises"]
ENV_WIDTHS = [80, 250, 210]
ENV_ROWS = [
    [
        "Identify OS ID",
        '. /etc/os-release; echo "${ID}-${VERSION_ID}"',
        "Match <os-id> directory (for example debian-13, rocky-8.10)",
    ],
    [
        "Activate tree",
        'source "$TREE/setEpicsEnv.bash"',
        "Set PATH, EPICS_BASE, and EPICS_MODULES in current shell",
    ],
    [
        "CA loopback",
        "export EPICS_CA_AUTO_ADDR_LIST=NO EPICS_CA_ADDR_LIST=127.0.0.1",
        "Send Channel Access UDP searches only to 127.0.0.1",
    ],
    [
        "PVA loopback",
        "export EPICS_PVA_AUTO_ADDR_LIST=NO EPICS_PVA_ADDR_LIST=127.0.0.1",
        "Send pvAccess UDP searches only to 127.0.0.1",
    ],
    [
        "Simulator ports",
        "ss -ltn '( sport >= :9399 and sport <= :9401 )'",
        "Verify exercise TCP ports 9399-9401 have no listener",
    ],
]

LOADER_TITLE = "2. Module Loader (iocsh.bash) CLI Options and Startup Directives"
LOADER_HEADERS = ["Invocation / Directive", "Long Form or Scope", "Behavior"]
LOADER_WIDTHS = [148, 120, 272]
LOADER_ROWS = [
    [
        "iocsh.bash st.cmd",
        "(default interactive)",
        "Resolve modules, generate /dev/fd/3 + /dev/fd/4, start softIocPVX",
    ],
    [
        "iocsh.bash -n st.cmd",
        "--show",
        "Print generated startup (dlload, dbd, macros) and exit without IOC",
    ],
    [
        "iocsh.bash -S st.cmd",
        "--non-interactive",
        "Run startup with the interactive iocsh prompt disabled",
    ],
    [
        "iocsh.bash -v st.cmd",
        "--verbose",
        "Echo startup commands as softIocPVX executes them",
    ],
    [
        'iocsh.bash -e "$TREE" st.cmd',
        '--environment "$TREE"',
        'Source "$TREE/setEpicsEnv.bash" inside loader process only',
    ],
    [
        "iocsh.bash -h",
        "--help",
        "Print loader command usage summary and exit",
    ],
    [
        "module <Name> [<Ver>]",
        "Top-level st.cmd only",
        "Load module + transitive deps; export <MACRO> from cfg/iocsh.conf",
    ],
    [
        'dbLoadGroup("<f>.json", "<M>")',
        "Before iocInit()",
        "Register QSRV2 structured PVA group PV (built into softIocPVX)",
    ],
]

IOCSH_TITLE = "3. IOC Shell (iocsh) Runtime Inspection and Control Commands"
IOCSH_HEADERS = ["IOC Shell Command", "Provider", "Inspection or Control Action"]
IOCSH_WIDTHS = [192, 72, 276]
IOCSH_ROWS = [
    [
        'dbl  /  dbl("<recordType>")',
        "EPICS Base",
        "List loaded record names (all records or one record type)",
    ],
    [
        'dbpr("<pv>", <level>)',
        "EPICS Base",
        'Print record fields at detail level 0-4 (for example dbpr("PV", 2))',
    ],
    [
        'asynReport(<level>, "<port>")',
        "asyn",
        "Show port connection state and counters (omit port for all drivers)",
    ],
    [
        "asynSetTraceMask(<port>, -1, ERROR|FLOW|DRIVER)",
        "asyn",
        "Enable driver and flow diagnostic messages on an asyn port",
    ],
    [
        'pvxgl(<level>, "<pattern>")',
        "PVXS / QSRV2",
        'List QSRV2 group PVs and atomic members (for example pvxgl(1, "*"))',
    ],
    [
        'reGrep("<regex>")',
        "retools",
        "List loaded record names matching a regular expression",
    ],
    [
        "exit",
        "EPICS Base",
        "Stop the IOC cleanly and return to the host Bash prompt",
    ],
]

CLIENT_TITLE = "4. Channel Access (CA) vs pvAccess (PVA) Client Commands"
CLIENT_HEADERS = ["Operation", "CA Client (Base)", "PVA Client (PVXS)", "Usage Notes"]
CLIENT_WIDTHS = [68, 114, 148, 210]
CLIENT_ROWS = [
    [
        "Read value",
        "caget <pv>",
        "pvxget <pv>",
        "pvxget prints Normative Type / group fields + alarm + time",
    ],
    [
        "Confirmed put",
        "caput -c <pv> <val>",
        'pvxput -r "record[block=true]" <pv> <val>',
        "Waits for async completion; plain pvxput writes non-blocking",
    ],
    [
        "Monitor",
        "camonitor -t sI <pv>",
        "pvxmonitor <pv>",
        "-t sI (capital I) prints incremental seconds; Ctrl+C stops",
    ],
    [
        "Inspect type",
        "caget -d CLASS_NAME <pv>",
        "pvxinfo <pv>",
        "CLASS_NAME prints record type; pvxinfo prints PVA layout",
    ],
    [
        "Check alarm",
        "caget <pv>.STAT <pv>.SEVR",
        "pvxget <pv>",
        "Confirm NO_ALARM; COMM / INVALID marks disconnected device",
    ],
]

WARNING_LINES = [
    "DIAGNOSTIC REMINDERS:  Place module directives only in top-level st.cmd (never inside iocshLoad fragments).",
    "On runtime errors, /dev/fd/4 line numbers match st.cmd directly; /dev/fd/3 holds generated loader commands.",
    "With commonIocsh autosave.iocsh, wait past SETTINGS_PERIOD (default 5 s) before stopping an IOC to persist settings.",
]

FOOTER_LEFT = "ALS-U EPICS Environment Training  |  EPICS-env 1.5.0 / Base 7.0.10"
FOOTER_RIGHT = "https://github.com/jeonghanlee/epics-trainings"


def check_fit(c, text, font_name, font_size, max_width, context):
    """Ensure rendered string width fits inside its allocated horizontal span."""
    rendered_w = c.stringWidth(text, font_name, font_size)
    if rendered_w > max_width:
        raise ValueError(
            f"Text overflow in {context}: {rendered_w:.1f} pt > {max_width:.1f} pt ({text!r})"
        )


def draw_header(c, y):
    """Draw the top dark banner with document title, metadata, and badge."""
    header_h = 54
    c.setFillColor(DARK_BG)
    c.rect(MARGIN, y - header_h, CONTENT_W, header_h, fill=True, stroke=False)

    xi = MARGIN + 10
    c.setFillColor(white)
    c.setFont("Helvetica-Bold", 13)
    check_fit(c, DOC_TITLE, "Helvetica-Bold", 13, CONTENT_W - 20, "header title")
    c.drawString(xi, y - 16, DOC_TITLE)

    c.setFont("Helvetica", 8)
    check_fit(c, DOC_SUBTITLE, "Helvetica", 8, CONTENT_W - 20, "header subtitle")
    c.drawString(xi, y - 28, DOC_SUBTITLE)

    c.setFont("Helvetica", 7)
    c.drawString(xi, y - 40, DOC_REF)
    c.setFont("Helvetica-Bold", 7.5)
    c.drawRightString(PAGE_W - MARGIN - 10, y - 40, DOC_BADGE)

    c.setFont("Helvetica", 6.5)
    c.drawString(xi, y - 50, DOC_AUTHOR)

    return y - header_h - SECTION_GAP


def draw_principle_box(c, y):
    """Draw the core activation and loopback summary callout box."""
    line_h = 10.5
    box_h = 16 + len(PRINCIPLE_LINES) * line_h + 6

    c.setFillColor(PRINCIPLE_BG)
    c.roundRect(MARGIN, y - box_h, CONTENT_W, box_h, 4, fill=True, stroke=False)

    c.setStrokeColor(ACCENT)
    c.setLineWidth(2)
    c.line(MARGIN + 4, y - 4, MARGIN + 4, y - box_h + 4)

    xi = MARGIN + 12
    c.setFillColor(DARK_BG)
    c.setFont("Helvetica-Bold", 8)
    c.drawString(xi, y - 12, PRINCIPLE_HEADING)

    c.setFont("Helvetica", 7)
    ty = y - 24
    for idx, line in enumerate(PRINCIPLE_LINES):
        check_fit(c, line, "Helvetica", 7, CONTENT_W - 22, f"principle line {idx + 1}")
        c.drawString(xi + 2, ty, line)
        ty -= line_h

    return y - box_h - SECTION_GAP


def draw_table(c, y, title, title_color, headers, rows, col_widths, row_h=12.2, header_h=13.5):
    """Draw a full-width left-aligned reference table with width verification."""
    c.setFillColor(DARK_BG)
    c.setFont("Helvetica-Bold", 8)
    check_fit(c, title, "Helvetica-Bold", 8, CONTENT_W, f"section title {title}")
    c.drawString(MARGIN, y, title)
    y -= TITLE_TO_TABLE_GAP

    scale = CONTENT_W / sum(col_widths)
    cols = [w * scale for w in col_widths]

    c.setFillColor(title_color)
    c.rect(MARGIN, y - header_h, CONTENT_W, header_h, fill=True, stroke=False)
    c.setFillColor(white)
    c.setFont("Helvetica-Bold", 6.3)

    cx = MARGIN
    for ci, header in enumerate(headers):
        check_fit(
            c,
            header,
            "Helvetica-Bold",
            6.3,
            cols[ci] - 2 * CELL_PAD,
            f"{title} header col {ci + 1}",
        )
        c.drawString(cx + CELL_PAD, y - 9.3, header)
        cx += cols[ci]
    y -= header_h

    table_top_y = y + header_h
    for ri, row in enumerate(rows):
        bg = TABLE_ROW_ALT if (ri % 2 == 0) else white
        c.setFillColor(bg)
        c.rect(MARGIN, y - row_h, CONTENT_W, row_h, fill=True, stroke=False)

        c.setStrokeColor(GRID_GRAY)
        c.setLineWidth(0.3)
        c.line(MARGIN, y - row_h, MARGIN + CONTENT_W, y - row_h)

        cx = MARGIN
        for ci, cell in enumerate(row):
            font_name = "Helvetica-Bold" if ci == 0 else "Helvetica"
            font_size = 6.0
            c.setFillColor(DARK_BG)
            c.setFont(font_name, font_size)
            check_fit(
                c,
                cell,
                font_name,
                font_size,
                cols[ci] - 2 * CELL_PAD,
                f"{title} row {ri + 1} col {ci + 1}",
            )
            c.drawString(cx + CELL_PAD, y - 8.6, cell)
            cx += cols[ci]
            if ci < len(row) - 1:
                c.setStrokeColor(GRID_GRAY)
                c.setLineWidth(0.3)
                c.line(cx, y, cx, y - row_h)
        y -= row_h

    total_h = table_top_y - y
    c.setStrokeColor(LIGHT_GRAY)
    c.setLineWidth(0.6)
    c.rect(MARGIN, y, CONTENT_W, total_h, fill=False, stroke=True)

    return y - SECTION_GAP


def draw_warning_box(c, y):
    """Draw the bottom diagnostic and persistence warning callout box."""
    line_h = 10.5
    box_h = 12 + len(WARNING_LINES) * line_h + 4

    c.setFillColor(WARN_BG)
    c.setStrokeColor(WARN_RED)
    c.setLineWidth(0.9)
    c.roundRect(MARGIN, y - box_h, CONTENT_W, box_h, 4, fill=True, stroke=True)

    xi = MARGIN + 10
    c.setFillColor(WARN_RED)
    c.setFont("Helvetica-Bold", 6.8)
    check_fit(c, WARNING_LINES[0], "Helvetica-Bold", 6.8, CONTENT_W - 20, "warning line 1")
    c.drawString(xi, y - 11, WARNING_LINES[0])

    c.setFillColor(DARK_BG)
    c.setFont("Helvetica", 6.4)
    for idx, line in enumerate(WARNING_LINES[1:], start=1):
        check_fit(c, line, "Helvetica", 6.4, CONTENT_W - 20, f"warning line {idx + 1}")
        c.drawString(xi, y - 11 - idx * line_h, line)

    return y - box_h - 10


def draw_footer(c, y):
    """Draw the bottom rule and document control footer."""
    if y - 10 < MARGIN - 6:
        raise ValueError(f"Vertical overflow: footer baseline {y - 10:.1f} pt below margin")

    c.setStrokeColor(LIGHT_GRAY)
    c.setLineWidth(0.5)
    c.line(MARGIN, y, PAGE_W - MARGIN, y)

    c.setFillColor(MUTED_GRAY)
    c.setFont("Helvetica", 6)
    c.drawString(MARGIN, y - 9, FOOTER_LEFT)
    c.drawRightString(PAGE_W - MARGIN, y - 9, FOOTER_RIGHT)


def generate_pdf(output_path=OUTPUT_PATH):
    """Render the single-page PDF quick reference card to output_path."""
    output_path = Path(output_path)
    output_path.parent.mkdir(parents=True, exist_ok=True)

    c = canvas.Canvas(str(output_path), pagesize=letter, invariant=1)
    c.setTitle(DOC_TITLE)
    c.setAuthor("Jeong Han Lee, Dr.rer.nat")
    c.setSubject("ALS-U EPICS Environment Quick Reference")

    y = PAGE_H - MARGIN
    y = draw_header(c, y)
    y = draw_principle_box(c, y)
    y = draw_table(c, y, ENV_TITLE, ACCENT, ENV_HEADERS, ENV_ROWS, ENV_WIDTHS)
    y = draw_table(c, y, LOADER_TITLE, TABLE_HEADER_BG, LOADER_HEADERS, LOADER_ROWS, LOADER_WIDTHS)
    y = draw_table(c, y, IOCSH_TITLE, ACCENT, IOCSH_HEADERS, IOCSH_ROWS, IOCSH_WIDTHS)
    y = draw_table(c, y, CLIENT_TITLE, OK_GREEN, CLIENT_HEADERS, CLIENT_ROWS, CLIENT_WIDTHS)
    y = draw_warning_box(c, y)
    draw_footer(c, y)

    c.save()
    return output_path


def main():
    output = generate_pdf()
    print(f"Generated: {output}")


if __name__ == "__main__":
    main()
