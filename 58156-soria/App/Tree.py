from __future__ import annotations
from typing import List


class Tree:

    def get_parent(self) -> Tree:
        pass

    def add_node(self, tree: Tree) -> None:
        pass

    def remove_node(self, tree: Tree) -> None:
        pass

    def is_parent(self) -> bool:
        pass


class Parent(Tree):

    def __init__(self, position) -> None:
        self.position = position
        self.parent = None
        self.children: List[Tree] = []

    def get_parent(self) -> Tree:
        return self.parent

    def add_node(self, tree: Tree) -> None:
        self.children.append(tree)
        tree.parent = self

    def remove_node(self, tree: Tree) -> None:
        self.children.remove(tree)

    def is_parent(self) -> bool:
        return True

    def __str__(self, level=0) -> str:
        ret = " "*level+"|-"+str(self.position)+"\n"
        for child in self.children:
            ret += child.__str__(level+1)
        return ret


class Child(Tree):

    def __init__(self, position) -> None:
        self.position = position
        self.parent = None

    def get_parent(self) -> Tree:
        return self.parent

    def is_parent(self) -> bool:
        return False

    def __str__(self, level=0) -> str:
        return " "*level+"|-"+str(self.position)+"\n"
