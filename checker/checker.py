import glob
import os
import sys
import subprocess
import time
buildpath = '/Users/k-takemr/ct-comp/IPOsolver/'
def next_combination(sub):
    x = (sub & -sub)
    y = sub + x
    return (((sub & ~y) // x) >> 1) | y

def buildSolver():
    subprocess.call(['make', '-C', buildpath])
    print('build end')
def getACTS():
    files = glob.glob("../benchmarks/ACTS/*")
    return files
def getCTWedge():
    files = glob.glob("../benchmarks/CTWedge/*")
    return files

def runBench(files, strength):
    results = {}
    for file in files:
        print(file)
        start = time.time()
        subprocess.call([buildpath+'IPOsolver', str(strength), file])
        calctime = time.time() - start
        # analyze the result
        # maesyori
        lines = []
        outfile = file.replace("../benchmarks", buildpath+"output")
        with open(outfile, 'r') as f:
            lines = f.readlines()
        testcases = len(lines)
        for i in range(len(lines)):
            lines[i] = lines[i].strip()
            lines[i] = lines[i].split(',')
            lines[i] = list(map(int, lines[i]))
        # fix the number of params
        numParams = len(lines[i])
        params = []
        for j in range(numParams):
            tmp = -1
            for i in range(len(lines)):
                tmp = max(tmp, lines[i][j])
            params.append(tmp+1)
        # kazoeage refs
        choice = (1<<strength) - 1
        refInteractionSet = []
        while choice < (1<<numParams):
            target = []
            for bit in range(numParams):
                if (choice & (1<<bit)) > 0:
                    target.append(bit)
            now = 1
            for ind in target:
                now *= params[ind]
            for permit in range(now):
                interaction = [-1]*numParams
                for ind in target:
                    interaction[ind] = permit%params[ind]
                    permit //= params[ind]
                interaction = [str(a) for a in interaction]
                refInteractionSet.append(','.join(interaction))
            choice = next_combination(choice)
        refInteractionSet = set(refInteractionSet)
        # kazoeage test
        testInteractionSet = []
        for test in lines:
            choice = (1<<strength) - 1
            while choice < (1<<numParams):
                target = []
                for bit in range(numParams):
                    if (choice & (1<<bit)) > 0:
                        target.append(bit)
                interaction = [-1]*numParams
                for ind in target:
                    interaction[ind] = test[ind]
                interaction = [str(a) for a in interaction]
                testInteractionSet.append(','.join(interaction))
                choice = next_combination(choice)
        testInteractionSet = set(testInteractionSet)
        res = (len(refInteractionSet) == len(testInteractionSet))
        results[file] = {'time':calctime, 'testcase':testcases, 'result':res}
    return results
def main():
    strength = int(sys.argv[1])
    buildSolver()
    results = runBench(getACTS(), strength)
    print(results)
    # runBench(getCTWedge(), strength)
main()