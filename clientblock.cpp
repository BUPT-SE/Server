#include "clientblock.h"

ClientBlock::ClientBlock()
{

}

Attribute ClientBlock::getAttribute(){
    return _attribute;
}

bool ClientBlock::isSatisfied(){
    return _satisfied;
}
