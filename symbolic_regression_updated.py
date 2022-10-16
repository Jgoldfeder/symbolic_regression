import random
import time
import matplotlib.pyplot as plt
import numpy as np
import ast,copy
import sys
from sympy import *
from math import *
import multiprocessing
        
def plot(points,best_tree,count):
    x=[]
    y=[]
    for p in points:
        x.append(p[0])
        y.append(p[1])
    y_2 = []
    for p in points:
        y_2.append(evaluate_tree(best_tree,p[0]))   
    plt.cla()     
    plt.plot(x,y)
    plt.plot(x,y_2)
    #plt.show()
    plt.savefig("img2/function"+str(count)+".png")
    
def read_data(dataset):
    count = 0

    points = []

    with open("data2022_"+dataset+".txt") as fp:
        Lines = fp.readlines()
        for line in Lines:
            toks = line.split(",")
            point = (float(toks[0]), float(toks[1]))
            count += 1
            points.append(point)
            
    print("read " + str(count) + " points from file!")
    return points
    
def get_tree(string):
    ast_exp = ast.parse(string)
    return ast_exp
    
def get_string(tree):
    return ast.unparse(tree)
    
def evaluate_tree(tree,x):
    x=x
    # if divide by zero return zero
    try:
        return eval(ast.unparse(tree))
    except ZeroDivisionError:
        return 0
        
def get_error(tree,points):
    dist = 0
    for p in points:
        dist += abs(evaluate_tree(tree,p[0]) - p[1])
    return dist/len(points)

def get_random_expression(max_depth=random.randint(0,4)):
    type = random.choices(["terminal","binary_operator","function"],weights =[0.2,0.6,0.2])[0]
    if type == "terminal" or max_depth==0:
       return random.choice([str(random.randrange(-10, 10)),"(0+x)","(0+x*x)","(1*x)","(1*x*x)"])
    elif type == "binary_operator":
        operator = random.choice(["+","-","/","*"])
        left = get_random_expression(max_depth-1)
        right = get_random_expression(max_depth-1)
        return "("+ left + operator + right + ")"        
    elif type == "function":
        f = random.choice(["cos","sin"])
        return f + "(" + get_random_expression(max_depth-1) + ")"

types = {}
types["binary_operator"] = "<class 'ast.BinOp'>"
types["constant"] = "<class 'ast.Constant'>"
types["variable"] = "<class 'ast.Name'>"
types["function"] = "<class 'ast.Call'>"
        
def mutate(tree,points):
    # copy for safety
    tree = copy.deepcopy(tree)
    
    root = tree.body[0].value
    constants = []
    operators = []
    variables = []
    functions = []
    
    queue = [root]
    while queue:
        node = queue.pop()
        node_type = str(type(node))
        if node_type == types["binary_operator"]:
            operators.append((node,root))
            queue.append(node.left)
            queue.append(node.right)
            root = node
            continue 
        elif node_type == types["constant"]:
            constants.append((node,root))
            root = node
            continue
        elif node_type == types["variable"]:
            variables.append((node,root))
            root = node
            continue            
        elif node_type == types["function"]:
            functions.append((node,root))
            queue.append(node.args[0]) # assume only 1 arg, since functions are sin, cos, etc
            root = node            
            continue 
    
    # mutate constants 
    num_constant_mutations = 30
    if len(constants) > 0:
        for _ in range(num_constant_mutations):
            # first pick a constant at random
            constant = random.choice(constants)[0]
            
            # give it a random value and keep if better
            original_value = constant.value
            error = get_error(tree,points)
            constant.value = random.uniform(-10, 10)
            #print(">>",error,get_string(tree))
            if get_error(tree,points) > error:
                # revert
                constant.value = original_value 
                
            # make small change and keep if better
            original_value = constant.value
            error = get_error(tree,points)
            constant.value += random.uniform(-.5, .5)
            if get_error(tree,points) > error:
                # revert
                constant.value = original_value



                
    return tree
    

def gradient_update(tree,points):
    # copy for safety
    tree = copy.deepcopy(tree)
    
    root = tree.body[0].value
    constants = []
    operators = []
    variables = []
    functions = []
    
    queue = [root]
    while queue:
        node = queue.pop()
        node_type = str(type(node))
        if node_type == types["binary_operator"]:
            operators.append((node,root))
            queue.append(node.left)
            queue.append(node.right)
            root = node
            continue 
        elif node_type == types["constant"]:
            constants.append((node,root))
            root = node
            continue
        elif node_type == types["variable"]:
            variables.append((node,root))
            root = node
            continue            
        elif node_type == types["function"]:
            functions.append((node,root))
            queue.append(node.args[0]) # assume only 1 arg, since functions are sin, cos, etc
            root = node            
            continue 
    
    # mutate constants 
    num_constant_mutations = 30
    if len(constants) > 0:
        for i in range(num_constant_mutations):
            error = get_error(tree,points)
            #print(i,error)
            for constant,_ in constants:
                original_value = constant.value
                constant.value +=.1
                error_if_larger = get_error(tree,points)
                if error_if_larger > error:
                   constant.value = original_value

                constant.value -=.1
                error_if_smaller = get_error(tree,points)
                if error_if_smaller > error:
                   constant.value = original_value                 
    return tree


def mutate_operator(tree):
    # copy for safety
    tree = copy.deepcopy(tree)
    
    root = tree.body[0].value
    constants = []
    operators = []
    variables = []
    functions = []
    
    queue = [root]
    while queue:
        node = queue.pop()
        node_type = str(type(node))
        if node_type == types["binary_operator"]:
            operators.append((node,root))
            queue.append(node.left)
            queue.append(node.right)
            root = node
            continue 
        elif node_type == types["constant"]:
            constants.append((node,root))
            root = node
            continue
        elif node_type == types["variable"]:
            variables.append((node,root))
            root = node
            continue            
        elif node_type == types["function"]:
            functions.append((node,root))
            queue.append(node.args[0]) # assume only 1 arg, since functions are sin, cos, etc
            root = node            
            continue 
    
    # change operator
    if len(operators) > 0:
        operator = random.choice(operators)[0]
        operator.op = random.choice([ast.Add(),ast.Sub(),ast.Mult(),ast.Div()])
    return tree 

def combine(tree1,tree2,operator):
    exp = "("+get_string(tree1) +") "+ operator + " (" + get_string(tree1) + ")"
    return get_tree(exp)

def get_nodes(tree):
    nodes = []
    root = tree.body[0].value  
    queue = [root]
    while queue:
        node = queue.pop()
        nodes.append(node)
        node_type = str(type(node))
        if node_type == types["binary_operator"]:
            queue.append(node.left)
            queue.append(node.right)
            continue 
        elif node_type == types["constant"]:
            continue
        elif node_type == types["variable"]:
            continue            
        elif node_type == types["function"]:
            queue.append(node.args[0]) # assume only 1 arg, since functions are sin, cos, etc
            continue
    return nodes

def get_terminals_in_subtree(tree):
    nodes = []
    root = tree
    queue = [root]
    while queue:
        node = queue.pop()
        
        node_type = str(type(node))
        if node_type == types["binary_operator"]:
            queue.append(node.left)
            queue.append(node.right)
            continue 
        elif node_type == types["constant"] or node_type == types["variable"]:
            nodes.append(node)
            continue          
        elif node_type == types["function"]:
            queue.append(node.args[0]) # assume only 1 arg, since functions are sin, cos, etc
            continue
    if len(nodes)==0:
        nodes.append(ast.Constant(1))
    return nodes
    
def crossover(tree1, tree2,fixed_pos=False):
    if random.choice([True,False]) and not fixed_pos:
        tmp = tree1
        tree1 = tree2
        tree2 = tmp
    
    # copy for safety
    tree = copy.deepcopy(tree1)
    
    root = tree.body[0].value
    constants = []
    operators = []
    variables = []
    functions = []
    
    queue = [root]
    while queue:
        node = queue.pop()
        node_type = str(type(node))
        if node_type == types["binary_operator"]:
            operators.append((node,root))
            queue.append(node.left)
            queue.append(node.right)
            root = node
            continue 
        elif node_type == types["constant"]:
            constants.append((node,root))
            root = node
            continue
        elif node_type == types["variable"]:
            variables.append((node,root))
            root = node
            continue            
        elif node_type == types["function"]:
            functions.append((node,root))
            queue.append(node.args[0]) # assume only 1 arg, since functions are sin, cos, etc
            root = node            
            continue  
    if len(operators+functions) > 0:
        node = random.choice(operators+functions)[0]
        node_type = str(type(node))
        cross_branch = random.choice(get_nodes(tree2))
        if node_type == types["binary_operator"]:    
            if random.choice([True,False]):
                node.left = cross_branch
            else:
                node.right = cross_branch
        else:
            node.args[0] = cross_branch
    return tree

def replace_with_new(tree):
    new_exp = random.choice([str(random.randrange(-10, 10)),"(0+x)","(0+x*x)","(1*x)","(1*x*x)"])
    new_tree = get_tree(new_exp)
    
    return crossover(tree,new_tree,True)

def prune(tree,max_depth=4):
    # copy for safety
    tree = copy.deepcopy(tree)
    root = tree.body[0].value  
    __prune(root,max_depth)
    return tree
    
def __prune(node,max_depth=4):
    
    node_type = str(type(node))
    if node_type == types["binary_operator"]:
        if max_depth == 0:
            node.left = random.choice(get_terminals_in_subtree(node.left))   
            node.right = random.choice(get_terminals_in_subtree(node.right)) 
        else:
            __prune(node.left,max_depth-1)
            __prune(node.right,max_depth-1)
            
    elif node_type == types["function"]:
        if max_depth == 0:
            node.args[0] = random.choice(get_terminals_in_subtree(node.args[0])) 
        else:
            __prune(node.args[0],max_depth-1)
            
    elif node_type == types["constant"] or node_type == types["variable"]:
        return 
        
def simplify_tree(tree):
    exp = get_string(tree)
    exp = str(simplify(exp))
    return get_tree(exp)
    
if False:
    exp1 = get_random_expression(2)
    tree1 = get_tree(exp1)
    exp2 = get_random_expression(2)
    tree2 = get_tree(exp2)

    print(get_string(tree1))
    print(get_string(tree2))

    tree1 = mutate_operator(tree1)
    tree2 = mutate_operator(tree2)

    print("mutated ops")
    print(get_string(tree1))
    print(get_string(tree2))

    
    tree1 = replace_with_new(tree1)
    tree2 = replace_with_new(tree2)

    print("replace new")
    print(get_string(tree1))
    print(get_string(tree2))    
    
    
    print("combined")
    print(get_string(combine(tree1,tree2,random.choice(["+","-","*","/"]))))

    print("crossover examples")
    print(get_string(crossover(tree1,tree2)))
    print(get_string(crossover(tree1,tree2)))
    print(get_string(crossover(tree1,tree2)))
    print(get_string(crossover(tree1,tree2)))
    
    print("prune")
    big_tree = combine(tree1,tree2,random.choice(["+","-","*","/"]))
    print("Original:",get_string(big_tree))
    print(" Simple:",get_string(simplify_tree(big_tree)))
    print("  Pruned:",get_string(prune(big_tree,3)))
    sys.exit(0)
    
class Process(multiprocessing.Process):
    def __init__(self, id,tree,points,return_dict):
        super(Process, self).__init__()
        self.id = id
        self.tree=tree
        self.points=points
        self.return_dict = return_dict
    def run(self):
        points=self.points
        tree = mutate(self.tree,points)
        tree = gradient_update(tree,points)
        tree = simplify_tree(tree)
        tree.error = get_error(tree,points) 
        self.return_dict[self.id] = tree


def main():        
    datasets = ["None","Gold","Silver","Bronze"]
    points = read_data(datasets[int(sys.argv[1])])
        
    start = time.time()

    population_size = 30
    generations = 1000
    population = []

    best_tree = None
    best_error = 10000000
    count = 0


            
          
    for i in range(population_size):
        exp = get_random_expression(3)
        tree = get_tree(exp)
        population.append(tree)

    manager = multiprocessing.Manager()
    return_dict = manager.dict()       
    for generation in range(generations):
        processes = []
        for i in range(len(population)):
            processes.append(Process(i,population[i],points,return_dict))
        for p in processes:
            p.start()
            
        for p in processes:
            p.join()
        print("generation:",generation,"total_elapsed_time:",time.time()-start)            
        for i in range(len(population)):
            population[i] = return_dict[i]
            print(i,population[i].error)
            
            if population[i].error < best_error:
                best_error = population[i].error
                best_tree = population[i]
                plot(points,best_tree,count) 
                count +=1
        print("error:",best_error," f(x)=",get_string(best_tree))
                
        # sort by error        
        population.sort(key=lambda x: x.error)
        # keep half
        population = population[:population_size//2]

        # make children

        new_pop = []
        for i in range(population_size//2):
            father = random.choice(population)
            mother = random.choice(population)
            
            if random.choice([True,False]):
                new_pop.append(crossover(father,mother))
            else:
                new_pop.append(replace_with_new(father))
            new_pop[-1] = prune(new_pop[-1],6)
        population += new_pop


    

if __name__ == '__main__':
    main()
    sys.exit(0)

if False:  
    for generation in range(generations):
        for i in range(len(population)):    
            tree = population[i]
            original_error = get_error(tree,points)
            tree = mutate(tree,points)
            tree = gradient_update(tree,points)
            new_error = get_error(tree,points)
            print(i,original_error,new_error, original_error-new_error,get_string(tree))
            tree = simplify_tree(tree)
            tree.error = new_error
            population[i]=tree

            if new_error < best_error:
                best_error = new_error
                best_tree = tree
                plot(points,best_tree,count) 
                count +=1
        plot(points,best_tree,count)  
        count +=1        
        # sort by error        
        population.sort(key=lambda x: x.error)
        # keep half
        population = population[:population_size//2]

        # make children

        new_pop = []
        for i in range(population_size//2):
            father = random.choice(population)
            mother = random.choice(population)
            
            if random.choice([True,False]):
                new_pop.append(crossover(father,mother))
            else:
                new_pop.append(replace_with_new(father))
            new_pop[-1] = prune(new_pop[-1])
        population += new_pop





        
    print("time elapsed:", time.time() - start,"best error:", get_error(best_tree,points), "f(x)=", get_string(best_tree))
    plot(points,best_tree,count)  
    #for i in range(10):
    #    print(get_random_expression())







