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
        self.func3_btn = ttk.Button(self.tabp_frame, text='Semantic',
                command=self.semantic_analysis)
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
        self.compile_finished = False

    def lex_analysis(self):
        if not self.compile_finished :
            if -1 == self.compile_file(self.source_file):
                return 
        data = []
        with open('../file/log/lex_analysis.log') as dataFile:
            for line in dataFile.readlines():
                if line == '\n':
                    continue
                line = line[:-1]
                data.append([s.strip() for s in line.split()])
        titles = [('String', 100), ('Type', 100), ('Code/Position', 100)]
        table = self.create_table(data, titles, self.show_frame)
        table.grid(padx=5, pady=5, row=0, column=0, rowspan=30, sticky='wens')

    def gramma_analysis(self):
        if not self.compile_finished :
            if -1 == self.compile_file(self.source_file):
                return 
        data = []
        status = 'Error'
        with open('../file/log/gramma_analysis.log') as dataFile:
            lines = dataFile.readlines()
            for line in lines:
                if line == '\n':
                    continue
                line = line[:-1]
                data.append([line])
        titles = [('ACTION', 100)]
        table = self.create_table(data, titles, self.show_frame)
        table.grid(padx=5, pady=5, row=0, column=0, rowspan=30, sticky='wens')
        reduce_btn = ttk.Button(self.show_frame, text='ReduceTable')
        shift_btn = ttk.Button(self.show_frame, text='ShiftTable')
        goto_btn = ttk.Button(self.show_frame, text='GotoTable')
        reduce_btn.grid(padx=5, pady=5, row=0, column=1, sticky='w')
        shift_btn.grid(padx=5, pady=5, row=1, column=1, sticky='wn')
        goto_btn.grid(padx=5, pady=5, row=2, column=1, sticky='wn')
        reduce_btn['command'] = self.show_reduce_table
        shift_btn['command'] = self.show_shift_table
        goto_btn['command'] = self.show_goto_table
        print(data[len(data)-1][0])
        if data[len(data)-1][0] == 'Accetped!':
            tkinter.messagebox.showinfo('Accetped', status)
        else :
            tkinter.messagebox.showerror('Error', data[len(data)-1][0]);

    def show_reduce_table(self):
        tmp_root = tkinter.Toplevel()
        tmp_root.title('Reduce Table')
        data = []
        with open('../file/log/ReduceTableInfo.log') as dataFile:
            for line in dataFile.readlines():
                if line == '\n':
                    continue
                line = line[:-1]
                data.append([s.strip() for s in line.split('\t')])
        titles = [('STATUS', 300), ('SEARCH', 300), ('REDUCE', 300)]
        table = self.create_table(data, titles, tmp_root)
        table['height'] = 25
        table.grid(padx=5, pady=5, row=0, column=0, sticky='wens')

    def show_shift_table(self):
        tmp_root = tkinter.Toplevel()
        tmp_root.title('SHIFT Table')
        data = []
        with open('../file/log/ShiftTableInfo.log') as dataFile:
            for line in dataFile.readlines():
                if line == '\n':
                    continue
                line = line[:-1]
                data.append([s.strip() for s in line.split('\t')])
        titles = [('STATUS', 300), ('MEET', 300), ('SHIFT_TO', 300)]
        table = self.create_table(data, titles, tmp_root)
        table['height'] = 25
        table.grid(padx=5, pady=5, row=0, column=0, sticky='wens')

    def show_goto_table(self):
        tmp_root = tkinter.Toplevel()
        tmp_root.title('GOTO Table')
        data = []
        with open('../file/log/GotoTableInfo.log') as dataFile:
            for line in dataFile.readlines():
                if line == '\n':
                    continue
                line = line[:-1]
                data.append([s.strip() for s in line.split('\t')])
        titles = [('STATUS', 300), ('MEET', 300), ('SHIFT_TO', 300)]
        table = self.create_table(data, titles, tmp_root)
        table['height'] = 25
        table.grid(padx=5, pady=5, row=0, column=0, sticky='wens')

    def semantic_analysis(self):
        if not self.compile_finished :
            if -1 == self.compile_file(self.source_file):
                return 
        data = []
        status = 'Error'
        with open('../file/log/Code.log') as dataFile:
            lines = dataFile.readlines()
            for line in lines:
                if line == '\n':
                    continue
                line = line[:-1]
                data.append([s for s in line.split('\t')])
        if len(data) == 1:
            tkinter.messagebox.showerror('Error!', 'Please check your source file!')
            return 
        titles = [('ACTION', 100), ('Arg_1', 100), ('Arg_2', 100), ('Result', 100)]
        table = self.create_table(data, titles, self.show_frame)
        table.grid(padx=5, pady=5, row=0, column=0, rowspan=30, sticky='wens')
        table_btn = ttk.Button(self.show_frame, text='SymbolTable')
        table_btn.grid(padx=5, pady=5, row=0, column=1, sticky='w')
        table_btn['command'] = self.show_new_symbol_table

    def show_new_symbol_table(self):
        tmp_root = tkinter.Toplevel()
        tmp_root.title('Symbol Table')
        data = []
        with open('../file/log/semantic_analysis.log') as dataFile:
            for line in dataFile.readlines():
                if line != '\n':
                    line = line[:-1]
                data.append([line])
        titles = [('Symbols', 300)]
        table = self.create_table(data, titles, tmp_root, False)
        table['height'] = 25
        table.grid(padx=5, pady=5, row=0, column=0, sticky='wens')
        pass

    def compile_file(self, filename):
        if filename == '':
            tkinter.messagebox.showerror('Error', 'Pleaes choose the source file first!')
            return -1
        else:
            self.compile_finished = True
            os.system('./main.out -n ' + filename)

    def create_table(self, data, colum_info, fill_frame, center=True):
        length = len(colum_info)
        colums_name = ['col' + str(i) for i in range(length)]
        table = ttk.Treeview(fill_frame, show='headings',
                columns=tuple(colums_name))
        for i in range(length):
            label = 'col' + str(i)
            val = colum_info[i][1]
            if (center):
                table.column(label, width=val, anchor='center')
            else: 
                table.column(label, width=val, anchor='w')
            table.heading(colums_name[i], text=colum_info[i][0])
        for i in range(len(data)):
            data_lst = [data[i][k] for k in range(length)]
            table.insert('', i, values=tuple(data_lst))
        return table # User should pack the table theirselves

def main():
    try :
        win = Compiler('Modnar C Compiler')
    except Exception as e:
        tkinter.messagebox.showerror('Error!', 'Please check your source file')

if __name__ == '__main__':
    main()
