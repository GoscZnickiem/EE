#ifndef EE_LIB_COMPILERUNIT_UNIT
#define EE_LIB_COMPILERUNIT_UNIT

#include "compiler_unit/Emitter.hpp"
#include "compiler_unit/Receiver.hpp"

namespace ee::cmpu {

template <typename ReceivedType, typename EmittedType>
class Unit : public Receiver<ReceivedType>,
			 public Emitter<EmittedType> {};

} // namespace ee::cmpu

#endif // !EE_LIB_COMPILERUNIT_UNIT
