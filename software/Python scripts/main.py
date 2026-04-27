import tkinter as tk

from go_board import GoBoardApp


if __name__ == "__main__":
    root = tk.Tk()
    app = GoBoardApp(root)
    root.mainloop()