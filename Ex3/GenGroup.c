// ------------------------------ includes ------------------------------
#include <assert.h>
#include "GenGroup.h"
#include <stdio.h>

// ------------------------------ functions -----------------------------

/*
 * @brief function that checks if a pointer is NULL
 * @param x pointer to GroupMember object
 * @return true if the pointer is NULL
 */
static bool isPointerNull(const GroupMemberP x)
{
    if(x == NULL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * @brief return the result of add(f(n),g(n))
 *   function name:addFunctions
 * @param add pointer to A function that gets 2 GroupMemberP objects and return a GroupMemberP
 * that is the result of the an opreation  on them.
 * @param f pointer to A function that gets a GroupMemberP and return a GroupMemberP
 * @param g pointer to A function that gets a GroupMemberP and return a GroupMemberP
 * @param n A  GroupMemberP
 * @param freeMember pointer to A function that get GroupMemberP and frees it from memory.
 * @return the result of add(f(n),g(n)).  In case of an error return NULL.
 *
 * runtime O(1)
 */
GroupMemberP addFunctions(const GroupMemberOperFuncP oper, const GroupMemberFuncP f,
                          const GroupMemberFuncP g, const GroupMemberP n,
                          const GroupMemberFreeFuncP freeMember)
{
    assert(oper != NULL && f != NULL && g != NULL && n !=NULL && freeMember !=NULL);
    GroupMemberP x, y, result;
    x = f(n);
    if(isPointerNull(x))
    {
        return NULL;
    }
    y = g(n);
    if(isPointerNull(y))
    {
        return NULL;
    }
    result = oper(x, y);
    if(isPointerNull(result))
    {
        return NULL;
    }
    freeMember(x);
    freeMember(y);
    return result;
}

/**
 * @brief return the result of f(g(n))
 *   function name:composeFunctions
 * @param f pointer to A function that gets a GroupMemberP and return a GroupMemberP
 * @param g pointer to A function that gets a GroupMemberP and return a GroupMemberP
 * @param n A  GroupMemberP
 * @param freeMember pointer to A function that get GroupMemberP and frees it from memory.
 * @return the result of f(g(n)).  In case of an error return NULL.
 *
 * runtime O(1)
 */
GroupMemberP composeFunctions(const GroupMemberFuncP f, const GroupMemberFuncP g,
                              const GroupMemberP n, const GroupMemberFreeFuncP freeMember)
{
    assert(f != NULL && g != NULL && n !=NULL && freeMember !=NULL);
    GroupMemberP x, result;
    x = g(n);
    if(isPointerNull(x))
    {
        return NULL;
    }
    result = f(x);
    if(isPointerNull(result))
    {
        return NULL;
    }
    freeMember(x);
    return result;
}

/*
 * @brief function that check if a given set satisfies the closure property
 * @param members array of pointers to GroupMember objects
 * @param membersLen the length of the members
 * @param oper is a pointer to a binary function that recevies two GroupMember pointer and returns
 *  one GroupMember pointer
 * @param freeMember pointer to a function that frees the mem alloc for GroupMemberP
 * @return true if the given set satisfies the property
 *
 * runtime O(n^3)
 */
static bool closureCheck(const GroupMemberP members[], const int membersLen,
                         const GroupMemberOperFuncP oper, const GroupMemberFreeFuncP freeMember,
                         const GroupMemberCompareFuncP compare)
{

    bool found;
    int k;
    GroupMemberP x = NULL;
    for(int i = 0; i < membersLen; i++)
    {
        for (int j = 0; j < membersLen; j++)
        {
            x = oper(members[i], members[j]);
            if(x == NULL)
            {
                return false;
            }
            k = 0;
            found = false;
            while(!found && k < membersLen)
            {
                // when compare returns 0 the compared elements are even

                if(!compare(x, members[k]))
                {
                    found = true;
                    freeMember(x);
                }
                k ++;
            }
            if(!found)
            {
                freeMember(x);
                return false;
            }
        }
    }
    return true;
}

/*
 * @brief function that check if a given set satisfies the associativity property
 * @param members array of pointers to GroupMember objects
 * @param membersLen the length of the members
 * @param oper is a pointer to a binary function that recevies two GroupMember pointer and returns
 *  one GroupMember pointer
 * @param freeMember pointer to a function that frees the mem alloc for GroupMemberP
 * @return true if the given set satisfies the property
 *
 * runtime O(n^3)
 */
static bool associativityCheck(const GroupMemberP members[], const int membersLen,
                               const GroupMemberOperFuncP oper,
                               const GroupMemberFreeFuncP freeMember,
                               const GroupMemberCompareFuncP compare)
{
    GroupMemberP result1;
    GroupMemberP result2;
    GroupMemberP subResult1;
    GroupMemberP subResult2;
    for (int i = 0; i < membersLen; i++)
    {
        for (int j = 0; j < membersLen; j++)
        {
            for (int k = 0; k < membersLen; k++)
            {
                subResult1 = oper(members[i], members[j]);
                result1 = oper(subResult1, members[k]);
                subResult2 = oper(members[j], members[k]);
                result2 = oper(members[i], subResult2);
                if(compare(result1, result2) || result1 == NULL || result2 == NULL)
                {
                    freeMember(result1);
                    freeMember(result2);
                    freeMember(subResult1);
                    freeMember(subResult2);
                    return false;
                }
                freeMember(result1);
                freeMember(result2);
                freeMember(subResult1);
                freeMember(subResult2);
            }
        }
    }
    return true;
}

/*
 * @brief function that check if a given GroupMemberP is an identity element inside a given set
 * @param identityElement the candidate to be the identity element
 * @param members array of pointers to GroupMember objects
 * @param membersLen the length of the members
 * @param oper is a pointer to a binary function that recevies two GroupMember pointer and returns
 *  one GroupMember pointer
 * @param freeMember pointer to a function that frees the mem alloc for GroupMemberP
 * @return true if the element is the identity element
 *
 * runtime O(n)
 */
static bool identityCheck(const GroupMemberP identityElement, const GroupMemberP members[],
                          const int membersLen, const GroupMemberOperFuncP oper,
                          const GroupMemberFreeFuncP freeMember,
                          const GroupMemberCompareFuncP compare)
{
    GroupMemberP result;
    for (int i = 0; i < membersLen; i++)
    {
        // there is a need to check the other way of oper(members[i],identityElement) but because
        // there will be a check for the commutativity of the set there is no need in this func
        // for the double check
        result = oper(identityElement, members[i]);
        if(compare(result, members[i]))
        {
            freeMember(result);
            return false;
        }
        freeMember(result);

    }
    return true;
}

/*
 * @brief function that check if a given GroupMemberP has an inverse element in the given set
 * @param identityElement the identity element in the set
 * @param a the element fro whom the inverse element is sought after
 * @param members array of pointers to GroupMember objects
 * @param membersLen the length of the members
 * @param oper is a pointer to a binary function that recevies two GroupMember pointer and returns
 *  one GroupMember pointer
 * @param freeMember pointer to a function that frees the mem alloc for GroupMemberP
 * @return true if the element has an inverse element inside the given set
 *
 * runtime O(n)
 */
static bool inverseElementCheckForElement(const GroupMemberP identityElement, const GroupMemberP a,
                                          const GroupMemberP members[], const int membersLen,
                                          const GroupMemberOperFuncP oper,
                                          const GroupMemberFreeFuncP freeMember,
                                          const GroupMemberCompareFuncP compare)
{
    GroupMemberP result;
    for (int i = 0; i < membersLen; i++)
    {

        result = oper(a, members[i]);
        if(!compare(result, identityElement))
        {
            freeMember(result);
            return true;
        }
        freeMember(result);
    }
    return false;
}

/*
 * @brief function that check if all GroupMemberP inside the set has an inverse element
 * @param identityElement the identity element in the set
 * @param members array of pointers to GroupMember objects
 * @param membersLen the length of the members
 * @param oper is a pointer to a binary function that recevies two GroupMember pointer and returns
 *  one GroupMember pointer
 * @param freeMember pointer to a function that frees the mem alloc for GroupMemberP
 * @return true if to all of the element an inverse element was found
 *
 * runtime O(n^2)
 */
static bool inverseCheck(const GroupMemberP identityElement, const GroupMemberP members[],
                         const int membersLen, const GroupMemberOperFuncP oper,
                         const GroupMemberFreeFuncP freeMember ,
                         const GroupMemberCompareFuncP compare)
{
    for (int i = 0; i < membersLen; i++)
    {
        if(!inverseElementCheckForElement(identityElement, members[i], members, membersLen, oper,
           freeMember, compare))
        {
            return false;
        }
    }
    return true;
}

/*
 * @brief function that check if a given set satisfies the commutativity property
 * @param members array of pointers to GroupMember objects
 * @param membersLen the length of the members
 * @param oper is a pointer to a binary function that recevies two GroupMember pointer and returns
 *  one GroupMember pointer
 * @param freeMember pointer to a function that frees the mem alloc for GroupMemberP
 * @return true if the given set satisfies the property
 *
 * runtime O(n^2)
 */
static bool commutativityCheck(const GroupMemberP members[], const int membersLen,
                               const GroupMemberOperFuncP oper,
                               const GroupMemberFreeFuncP freeMember,
                               const GroupMemberCompareFuncP compare)
{
    GroupMemberP result1;
    GroupMemberP result2;
    for (int i = 0; i < membersLen; i++)
    {
        for (int j = 0; j < membersLen; j++)
        {
            result1 = oper(members[i], members[j]);
            result2 = oper(members[j], members[i]);
            if(compare(result1, result2))
            {
                freeMember(result1);
                freeMember(result2);
                return false;
            }
            freeMember(result1);
            freeMember(result2);
        }
    }
    return true;
}

/*
 * @brief function that check if a given candidate to be identity element is inside the set
 * @param members array of pointers to GroupMember objects
 * @param membersLen the length of the members
 * @return true if element is inside the set
 *
 * runtime O(n)
 */
static bool isIdentityElementInGroup(const GroupMemberP identityElement,
                                     const GroupMemberP members[], const int membersLen,
                                     const GroupMemberCompareFuncP compare)
{
    for (int i = 0; i < membersLen; i++)
    {
        if(!compare(identityElement, members[i]))
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief check if the given arguments define a an Abelian Group
 * you should follow the definition of Abelian group -
 * see https://en.wikipedia.org/wiki/Abelian_group.
 * if some arguments are NULL print the following error message to stderr :
 * "Error : <argument_name> is NULL\n"
 * @param IdentityElement - The  identity element.
 * @param members A struct of GroupMemberP which contain ALL the group members
 * (including IdentityElement).
 * @param membersLen The length of members.
 * @param oper pointer to A function that gets 2 GroupMemberP objects and return a GroupMemberP
 * that is the result of the opreation  on them.
 * @param freeMember pointer to A function that get GroupMemberP and frees it from memory.
 * @param compare pointer to A function that gets 2 GroupMemberP objects and
 * return int >0 iff the first is bigger;
 * <0 if the second is bigger; and 0 iff they are equal(i.e.
 * the diffrents beetween the diffrent between them is <EPSILON).
 * @return true iff the pramters an Abelian Group group.  In case of an error return false.
 *
 * runtime O(n^3)
 */
bool isAbelianGroup(const GroupMemberP IdentityElement, const GroupMemberP members[],
                    int membersLen, const GroupMemberOperFuncP oper,
                    const GroupMemberFreeFuncP freeMember, const GroupMemberCompareFuncP compare)
{
    // membersLen can be greater than the length of the members array
    for(int i = 0; i < membersLen ; i++)
    {
        if(members[i] == NULL)
        {
            membersLen = i;
            break;
        }
    }

    if(!isIdentityElementInGroup(IdentityElement, members, membersLen, compare))
    {
        return false;
    }
    if(!closureCheck(members, membersLen, oper, freeMember, compare))
    {
        return false;
    }
    if(!associativityCheck(members, membersLen, oper, freeMember, compare))
    {
        return false;
    }
    if(!commutativityCheck(members, membersLen, oper, freeMember, compare))
    {
        return false;
    }
    if(!identityCheck(IdentityElement, members, membersLen, oper, freeMember, compare))
    {
        return false;
    }
    if(!inverseCheck(IdentityElement, members, membersLen, oper, freeMember, compare))
    {
        return false;
    }
    return true;
}

