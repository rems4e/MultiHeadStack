# MultiHeadStack

A generic trie-like structure that supports push and pop operations, like a stack.

[![Build Status](https://jenkins.remi-saurel.com/buildStatus/icon?job=MultiHeadStack)](https://jenkins.remi-saurel.com/job/MultiHeadStack/)

## Behavior

A push operation returns a reference to the newly inserted node. If we want to grow the stack, the next push operation
has to occur on the returned node. If we want to had a new "head" to the stack, the push operation has to occur on the
same node as before, instead.

### Push
If we first push a value `A` to the stack, we get the following structure:
```
A
```

Let's perform a push of `B` on the node `A`:
```
A __ B
```

Then, a push of `C` on the node `B`:
```
A __ B __ C
```

Up to now, it acts as a classic stack.

However, if we push `D` to `A`, we get this:
```
A __ B __ C
 |
 |__ D
```

The stack has another head, where we can continue to push, `E` for instance:

```
A __ B __ C
 |
 |__ D __ E
```

### Pop

To continue illustrating the structure's mechanism, push `F`, then `G` to node `D`:
```
A __ B __ C
 |
 |__ D __ E
      |
      |__ F __ G
```

Then, we can look at two scenarii:
* either we pop `E`, and we get this structure:
```
A __ B __ C
 |
 |__ D __ F __ G
```
* or we pop `G` then `F`, and we get back to this structure:
```
A __ B __ C
 |
 |__ D __ E
```

Nothing unexpected here.

A final notice is that popping a node that still has children is a no-op.
