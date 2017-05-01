//
// Created by Нико on 21.04.2017.
//

#ifndef ACR_DOMAINOBJECT_H
#define ACR_DOMAINOBJECT_H

#include <cstdlib>
#include <memory>

class DomainObject {
public:
    virtual void Update(double) = 0;

protected:
    virtual ~DomainObject() {}
};

typedef std::shared_ptr<DomainObject> DomainObject_ptr;

#endif //ACR_DOMAINOBJECT_H
