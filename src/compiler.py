#!/usr/bin/env python3
# -*- coding: UTF-8 -*-

import tkinter
import os
from tkinter import ttk

class MainWindow(object):
    def __init__(self, label):
        self.root = tkinter.Tk()
        self.root.title(label)
        self.labe_frame = ttk.Frame(self.root)
        self.show_frame = ttk.Frame(self.root)
        self.oper_frame = ttk.Frame(self.root)
        self.labe_frame.grid(padx=10, pady=10, row=0, column=0, sticky='we')
        self.show_frame.grid(padx=10, pady=10, row=1, column=0, sticky='we')
        self.oper_frame.grid(padx=10, pady=10, row=2, column=0, sticky='we')
        self.add_widgets()
        self.root.mainloop()

    def add_widgets(self):
        title_label = ttk.Label(self.labe_frame, text='Modnar C Compiler', 
                font=('SF-Mono', 28, 'bold'))
        title_label.grid(padx=10, pady=10, row=0, column=0, sticky='we')

        self.show_text = tkinter.Text(self.show_frame, font=('SF-Mono', 14))
        self.show_text.grid(padx=10, pady=10, row=0, column=0, sticky='we')

        self.srcf_entry = ttk.Entry(self.oper_frame) 
        comp_btn = ttk.Button(self.oper_frame, text='Compile!', 
                command=self.do_compile) 
        self.srcf_entry.grid(padx=10, pady=10, row=0, column=0, sticky='we')
        comp_btn.grid(padx=10, pady=10, row=0, column=1, sticky='e')

    def do_compile(self):
        filename = self.srcf_entry.get()
        cmd = './res.out -n ' + filename
        outputfile = '../file/log/info.log'
        os.system(cmd)
        with open(outputfile) as inputfile:
            self.show_text.delete('1.0', tkinter.END)
            self.show_text.insert(tkinter.INSERT, inputfile.read())

def main():
    win = MainWindow('Compiler')

if __name__ == '__main__':
    main()
