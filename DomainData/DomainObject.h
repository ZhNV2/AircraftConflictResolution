//
// Created by Нико on 21.04.2017.
//

#ifndef ACR_DOMAINOBJECT_H
#define ACR_DOMAINOBJECT_H

#include <cstdlib>

class DomainObject {
public:
    virtual void Update(double) = 0;

protected:
    virtual ~DomainObject() {}
};

#endif //ACR_DOMAINOBJECT_H
