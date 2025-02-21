#ifndef PERDEROS_H
#define PERDEROS_H

#include <CryEntitySystem/IEntityComponent.h>

class Perderos final : public IEntityComponent {
public:
	friend void ReflectType(Schematyc::CTypeDesc<Perderos>& desc);

private:
	void Initialize() override;
};

#endif // !PERDEROS_H