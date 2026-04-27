import tkinter as tk

from config import BOARD_SIZE, SERIAL_DELAY_MS
from arduino_serial import ArduinoSerial


class GoBoardApp:
    def __init__(self, root):
        self.root = root
        self.serial = ArduinoSerial()

        self.board_size = BOARD_SIZE
        self.board_state = [["" for _ in range(self.board_size)] for _ in range(self.board_size)]
        self.intersections = []

        self.serial_busy = False
        self.serial_delay_ms = SERIAL_DELAY_MS
        self.arduino_send_enabled = tk.BooleanVar(value=True)

        self.setup_window()
        self.setup_layout()
        self.build_board()
        self.build_right_panel()

    # -------------------------
    # Setup
    # -------------------------

    def setup_window(self):
        self.root.title("Responsive 5x5 Go Board")
        self.root.geometry("1000x700")

    def setup_layout(self):
        self.root.grid_rowconfigure(0, weight=1)
        self.root.grid_columnconfigure(0, weight=1)
        self.root.grid_columnconfigure(1, weight=2)
        self.root.grid_columnconfigure(2, weight=1)

        self.left_frame = tk.Frame(self.root, bg="white")
        self.left_frame.grid(row=0, column=0, sticky="nsew")

        self.middle_frame = tk.Frame(self.root, bg="white")
        self.middle_frame.grid(row=0, column=1, sticky="nsew")

        self.right_frame = tk.Frame(self.root, bg="white")
        self.right_frame.grid(row=0, column=2, sticky="nsew")

        self.middle_frame.grid_rowconfigure(0, weight=1)
        self.middle_frame.grid_columnconfigure(0, weight=1)

    def build_board(self):
        self.canvas = tk.Canvas(
            self.middle_frame,
            bg="white",
            highlightthickness=0
        )
        self.canvas.grid(row=0, column=0, sticky="nsew", padx=20, pady=20)

        self.canvas.bind("<Configure>", self.redraw_board)
        self.canvas.bind("<Button-1>", lambda event: self.handle_board_click(event, "black"))
        self.canvas.bind("<Button-3>", lambda event: self.handle_board_click(event, "white"))

    def build_right_panel(self):
        panel = self.right_frame

        panel.grid_rowconfigure(0, weight=1)
        panel.grid_rowconfigure(2, weight=1)
        panel.grid_columnconfigure(0, weight=1)

        container = tk.Frame(panel, bg="white")
        container.grid(row=1, column=0, padx=16)

        tk.Label(
            container,
            text="Arduino",
            bg="white",
            fg="#444444",
            font=("Helvetica", 11, "bold")
        ).pack(pady=(0, 8))

        self.toggle_btn = tk.Button(
            container,
            text="● ENABLED",
            font=("Helvetica", 10, "bold"),
            width=12,
            relief="flat",
            cursor="hand2",
            command=self.toggle_arduino_send,
            **self.button_style(enabled=True)
        )
        self.toggle_btn.pack()

        self.reset_btn = tk.Button(
            container,
            text="RESTART",
            font=("Helvetica", 10, "bold"),
            width=12,
            relief="flat",
            cursor="hand2",
            command=self.reset_board_and_boxes,
            bg="#1565c0",
            fg="white",
            activebackground="#0d47a1",
            activeforeground="white"
        )
        self.reset_btn.pack(pady=(12, 0))

        self.status_label = tk.Label(
            container,
            text="Sending to robot",
            bg="white",
            fg="#2e7d32",
            font=("Helvetica", 8)
        )
        self.status_label.pack(pady=(6, 0))

    # -------------------------
    # UI helpers
    # -------------------------

    def button_style(self, enabled: bool) -> dict:
        if enabled:
            return {
                "bg": "#2e7d32",
                "fg": "white",
                "activebackground": "#1b5e20",
                "activeforeground": "white"
            }

        return {
            "bg": "#e0e0e0",
            "fg": "#888888",
            "activebackground": "#bdbdbd",
            "activeforeground": "#666666"
        }

    def set_controls_enabled(self, enabled: bool):
        state = tk.NORMAL if enabled else tk.DISABLED

        self.toggle_btn.config(state=state)
        self.reset_btn.config(state=state)

    def toggle_arduino_send(self):
        enabled = not self.arduino_send_enabled.get()
        self.arduino_send_enabled.set(enabled)

        if enabled:
            self.toggle_btn.config(text="● ENABLED", **self.button_style(True))
            self.status_label.config(text="Sending to robot", fg="#2e7d32")
        else:
            self.toggle_btn.config(text="○ DISABLED", **self.button_style(False))
            self.status_label.config(text="Board only", fg="#999999")

    # -------------------------
    # Board drawing
    # -------------------------

    def redraw_board(self, event=None):
        self.canvas.delete("all")
        self.intersections.clear()

        canvas_width = self.canvas.winfo_width()
        canvas_height = self.canvas.winfo_height()

        board_pixel_size = int(min(canvas_width, canvas_height) * 0.8)

        left = (canvas_width - board_pixel_size) / 2
        top = (canvas_height - board_pixel_size) / 2
        right = left + board_pixel_size
        bottom = top + board_pixel_size

        border_margin = board_pixel_size * 0.12

        grid_left = left + border_margin
        grid_top = top + border_margin
        grid_right = right - border_margin
        grid_bottom = bottom - border_margin

        self.canvas.create_rectangle(
            left, top, right, bottom,
            fill="#DABB85",
            outline="#DABB85",
            width=2
        )

        spacing = (grid_right - grid_left) / (self.board_size - 1)
        stone_radius = spacing * 0.4

        self.draw_grid(grid_left, grid_top, grid_right, grid_bottom, spacing)
        self.draw_stones(grid_left, grid_top, spacing, stone_radius)

    def draw_grid(self, grid_left, grid_top, grid_right, grid_bottom, spacing):
        for i in range(self.board_size):
            x = grid_left + i * spacing
            self.canvas.create_line(x, grid_top, x, grid_bottom, width=2)

        for i in range(self.board_size):
            y = grid_top + i * spacing
            self.canvas.create_line(grid_left, y, grid_right, y, width=2)

    def draw_stones(self, grid_left, grid_top, spacing, stone_radius):
        for row in range(self.board_size):
            for col in range(self.board_size):
                x = grid_left + col * spacing
                y = grid_top + row * spacing

                state = self.board_state[row][col]

                if state:
                    self.canvas.create_oval(
                        x - stone_radius,
                        y - stone_radius,
                        x + stone_radius,
                        y + stone_radius,
                        fill=state,
                        outline=""
                    )

                self.intersections.append({
                    "row": row,
                    "col": col,
                    "x": x,
                    "y": y,
                    "radius": stone_radius
                })

    # -------------------------
    # Board interaction
    # -------------------------

    def handle_board_click(self, event, colour):
        if self.arduino_send_enabled.get() and self.serial_busy:
            return

        point = self.get_clicked_intersection(event.x, event.y)

        if point is None:
            return

        row = point["row"]
        col = point["col"]

        current = self.board_state[row][col]
        tag = self.get_position_tag(row, col)

        if current == "":
            self.board_state[row][col] = colour
            action = "place"
        else:
            self.board_state[row][col] = ""
            action = "remove"

        self.redraw_board()

        if self.arduino_send_enabled.get():
            self.send_tag_to_arduino(tag, action)

    def get_clicked_intersection(self, x, y):
        for point in self.intersections:
            dx = x - point["x"]
            dy = y - point["y"]

            if dx * dx + dy * dy <= point["radius"] ** 2:
                return point

        return None

    def get_position_tag(self, row, col):
        return f"{chr(ord('a') + col)}{row + 1}"

    # -------------------------
    # Serial delay
    # -------------------------

    def start_serial_delay(self):
        self.serial_busy = True
        self.set_controls_enabled(False)
        self.status_label.config(text="Busy...", fg="#ff9800")

        self.root.after(self.serial_delay_ms, self.finish_serial_delay)

    def finish_serial_delay(self):
        self.serial_busy = False
        self.set_controls_enabled(True)

        if self.arduino_send_enabled.get():
            self.status_label.config(text="Sending to robot", fg="#2e7d32")
        else:
            self.status_label.config(text="Board only", fg="#999999")

    # -------------------------
    # Serial commands
    # -------------------------

    def send_tag_to_arduino(self, tag, action):
        if self.serial_busy:
            return

        if action == "remove":
            tag += "r"
        elif action == "place":
            tag += "p"

        self.serial.send(tag)
        self.start_serial_delay()

    def reset_board_and_boxes(self):
        if self.serial_busy:
            return

        self.serial.send("resetbox")
        self.clear_board()
        self.start_serial_delay()

    def clear_board(self):
        self.board_state = [["" for _ in range(self.board_size)] for _ in range(self.board_size)]
        self.redraw_board()