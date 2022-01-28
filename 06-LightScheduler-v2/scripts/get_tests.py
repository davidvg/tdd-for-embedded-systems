from os import listdir
import argparse
import re


def get_all_files(test_path):
    files = listdir(test_path)
    return files

def filter_test_files(file_list):
    p = re.compile('.*Test.cpp$')
    res = []
    for f in file_list:
        m = p.match(f)
        if(m):
            res.append(m[0])
    return res
    
def get_test_files(test_path):
    all_files = get_all_files(test_path)
    test_files = filter_test_files(all_files)
    return test_files





def extract_all_tests(file_list, output):
    res = {}
    tmp = []
    p = re.compile('^TEST\(')
    for file_ in file_list:
        with open(args.path + '/' + file_, 'r') as f:
            pass


if __name__ == '__main__':
    argparser = argparse.ArgumentParser()
    argparser.add_argument('-p', '--path')
    argparser.add_argument('-o', '--output', default='tests.txt')
    args = argparser.parse_args()
    
    test_files = get_test_files(args.path)
    extract_all_tests(test_files, args.output)