#!/usr/bin/env python3
# -*- coding: UTF-8 -*-

import tkinter
import os
from tkinter import ttk
import tkinter.messagebox
import tkinter.filedialog

class Compiler(object):
    def __init__(self, label):
        self.root = tkinter.Tk()
        self.root.title(label)
        self.add_widgets()
        self.source_file = ''
        self.compile_finished = False
        self.root.mainloop()

    def add_widgets(self):
        self.tabp_frame = ttk.Frame(self.root)
        self.show_frame = ttk.Frame(self.root)
        self.oper_frame = ttk.Frame(self.root)
        self.tabp_frame.grid(padx=5, pady=10, row=0, column=0, rowspan=10, 
                sticky='nswe')
        self.show_frame.grid(padx=5, pady=10, row=0, column=1, sticky='nswe')
        self.oper_frame.grid(padx=5, pady=10, row=1, column=1, sticky='we')

        self.show_text = tkinter.Text(self.show_frame, font=('SF-Mono', 14))
        self.show_text.grid(padx=10, pady=10, row=0, column=0, sticky='we')

        self.func1_btn = ttk.Button(self.tabp_frame, text='Lex', 
                command=self.lex_analysis)
        self.func2_btn = ttk.Button(self.tabp_frame, text='Gramma', 
                command=self.gramma_analysis)
        self.func3_btn = ttk.Button(self.tabp_frame)
        self.func1_btn.grid(padx=10, pady=10, row=0, column=0, sticky='we')
        self.func2_btn.grid(padx=10, pady=10, row=1, column=0, sticky='we')
        self.func3_btn.grid(padx=10, pady=10, row=2, column=0, sticky='we')

        self.filename = tkinter.StringVar()
        self.file_entry = ttk.Entry(self.oper_frame, textvariable = self.filename, width=65)
        self.file_entry.grid(padx=10, pady=10, row = 0, column = 0)
        select_btn = ttk.Button(self.oper_frame, text='浏览', 
                command=self.select_file)
        select_btn.grid(padx=10, pady=10, row=0, column=1)

    def select_file(self):
        path_ = tkinter.filedialog.askopenfilename()
        self.filename.set(path_)
        self.source_file = self.filename.get()

    def lex_analysis(self):
        if not self.compile_finished :
            self.compile_file(self.source_file)
        else :
            pass

    def gramma_analysis(self):
        if not self.compile_finished :
            self.compile_file(self.source_file)
        pass

    def compile_file(self, filename):
        if filename == '':
            tkinter.messagebox.showerror('Error', 'Pleaes choose the source file first!')
            return 
        else:
            self.compile_finished = True

def main():
    win = Compiler('Modnar C Compiler')

if __name__ == '__main__':
    main()
