//* This file is part of the RACCOON application
//* being developed at Dolbow lab at Duke University
//* http://dolbow.pratt.duke.edu

#pragma once

// MOOSE includes
#include "ElementUserObject.h"

template <bool is_ad>
class MaterialPropertyUserObjectTempl : public ElementUserObject
{
public:
  static InputParameters validParams();

  MaterialPropertyUserObjectTempl(const InputParameters & parameters);

  virtual void initialSetup() override;
  virtual void meshChanged() override;
  virtual void initialize() override {}
  virtual void execute() override { computeProperties(); }
  virtual void threadJoin(const UserObject & /*y*/) override {}
  virtual void finalize() override {}
  virtual Real getValue()
  {
    mooseError(name() + " does not return a postprocessor value");
    return 0;
  }

  GenericReal<is_ad> getData(const Elem * e, unsigned int qp) const { return _to[e->id()][qp]; }

protected:
  virtual void computeProperties();

  unsigned int _qp;

  const GenericMaterialProperty<Real, is_ad> & _from;

  std::vector<std::vector<GenericReal<is_ad>>> _to;
};

typedef MaterialPropertyUserObjectTempl<false> MaterialPropertyUserObject;
typedef MaterialPropertyUserObjectTempl<true> ADMaterialPropertyUserObject;
