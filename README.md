# Блок-схемы алгоритмов (ГОСТ 19.701-90)

Ниже представлены блок-схемы ключевых алгоритмов программы, реализованные в синтаксисе Mermaid.

## 1. Вставка узла в BST (InsertBST)
Алгоритм рекурсивно ищет место для вставки. Особенность: при совпадении ключа (фамилии) новый индекс добавляется в список индексов существующего узла (обработка однофамильцев).

```mermaid
flowchart TD
    Start([Начало: InsertBST]) --> Input[/Вх: node, key, index/]
    Input --> CheckNull{node == nullptr?}

    CheckNull -- Да --> CreateNode[Создать новый узел<br>с key и index]
    CreateNode --> ReturnNew[/Вернуть указатель<br>на новый узел/]

    CheckNull -- Нет --> CheckKey{key == node->key?}

    CheckKey -- Да --> AddIndex[Добавить index<br>в node->indices]
    AddIndex --> ReturnNode[/Вернуть node/]

    CheckKey -- Нет --> CheckLess{key < node->key?}

    CheckLess -- Да --> CallLeft[[node->left = InsertBST<br>node->left, key, index]]
    CallLeft --> ReturnNode

    CheckLess -- Нет --> CallRight[[node->right = InsertBST<br>node->right, key, index]]
    CallRight --> ReturnNode

    ReturnNew --> End([Конец])
    ReturnNode --> End
```

## 2. Итерационный поиск в BST (IterSearchBST)
Поиск выполняется в цикле без использования стека вызовов, что экономит память.

```mermaid
flowchart TD
    Start([Начало: IterSearchBST]) --> Input[/Вх: root, key/]
    Input --> SetCur[Текущий узел cur = root]

    LoopStart{cur != nullptr?}

    LoopStart -- Нет --> NotFound[/Вернуть пустой список/]
    NotFound --> End([Конец])

    LoopStart -- Да --> CheckMatch{key == cur->key?}

    CheckMatch -- Да --> Found[/Вернуть cur->indices/]
    Found --> End

    CheckMatch -- Нет --> CheckLess{key < cur->key?}

    CheckLess -- Да --> MoveLeft[cur = cur->left]
    MoveLeft --> LoopStart

    CheckLess -- Нет --> MoveRight[cur = cur->right]
    MoveRight --> LoopStart
```

## 3. Рекурсивный поиск в BST (RecSearchBST)
Классический алгоритм спуска по дереву.

```mermaid
flowchart TD
    Start([Начало: RecSearchBST]) --> Input[/Вх: node, key, out_vector/]
    Input --> CheckNull{node == nullptr?}

    CheckNull -- Да --> ReturnVoid([Возврат])

    CheckNull -- Нет --> CheckMatch{key == node->key?}

    CheckMatch -- Да --> AddFound[Добавить node->indices<br>в out_vector]
    AddFound --> ReturnVoid

    CheckMatch -- Нет --> CheckLess{key < node->key?}

    CheckLess -- Да --> CallLeft[[RecSearchBST<br>node->left, key, out]]
    CallLeft --> ReturnVoid

    CheckLess -- Нет --> CallRight[[RecSearchBST<br>node->right, key, out]]
    CallRight --> ReturnVoid
```

## 4. Физическое удаление записей (PackArray)
Алгоритм "уплотнения" массива. Переносит все не удаленные (DeletedMask == false) записи в начало массива, перезаписывая удаленные.

```mermaid
flowchart TD
    Start([Начало: PackArray]) --> InitVars[newCount = 0<br>i = 0]

    LoopCheck{i < SdntCount?}

    LoopCheck -- Нет --> UpdateCount[SdntCount = newCount]
    UpdateCount --> RebuildIndex[[Перестроить индексы<br>и дерево]]
    RebuildIndex --> End([Конец])

    LoopCheck -- Да --> CheckDeleted{DeletedMask i == true?}

    CheckDeleted -- Да (удален) --> IncI[i = i + 1]
    IncI --> LoopCheck

    CheckDeleted -- Нет (живой) --> CopyRec[SdntArr newCount = SdntArr i]
    CopyRec --> ResetMask[DeletedMask newCount = false]
    ResetMask --> IncNew[newCount = newCount + 1]
    IncNew --> IncI
```
