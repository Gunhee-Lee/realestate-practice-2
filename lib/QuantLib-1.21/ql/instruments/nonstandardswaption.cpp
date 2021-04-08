/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2013, 2018 Peter Caspers

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include <ql/instruments/nonstandardswaption.hpp>
#include <ql/exercise.hpp>

namespace QuantLib {

    NonstandardSwaption::NonstandardSwaption(const Swaption &fromSwaption)
        : Option(ext::shared_ptr<Payoff>(),
                 const_cast<Swaption &>(fromSwaption).exercise()),
          swap_(ext::make_shared<NonstandardSwap>(
              *fromSwaption.underlyingSwap())),
          settlementType_(fromSwaption.settlementType()),
          settlementMethod_(fromSwaption.settlementMethod()) {

        registerWith(swap_);
    }

    NonstandardSwaption::NonstandardSwaption(
        const ext::shared_ptr<NonstandardSwap>& swap,
        const ext::shared_ptr<Exercise>& exercise, Settlement::Type delivery,
        Settlement::Method settlementMethod)
        : Option(ext::shared_ptr<Payoff>(), exercise), swap_(swap),
          settlementType_(delivery), settlementMethod_(settlementMethod) {
        registerWith(swap_);
        registerWithObservables(swap_);
    }

    bool NonstandardSwaption::isExpired() const {

        return detail::simple_event(exercise_->dates().back()).hasOccurred();
    }

    void
    NonstandardSwaption::setupArguments(PricingEngine::arguments *args) const {

        swap_->setupArguments(args);

        NonstandardSwaption::arguments *arguments =
            dynamic_cast<NonstandardSwaption::arguments *>(args);

        QL_REQUIRE(arguments != 0, "argument types do not match");

        arguments->swap = swap_;
        arguments->exercise = exercise_;
        arguments->settlementType = settlementType_;
        arguments->settlementMethod = settlementMethod_;
    }

    void NonstandardSwaption::arguments::validate() const {

        NonstandardSwap::arguments::validate();
        QL_REQUIRE(swap, "underlying non standard swap not set");
        QL_REQUIRE(exercise, "exercise not set");
        Settlement::checkTypeAndMethodConsistency(settlementType,
                                                  settlementMethod);
    }

    Disposable<std::vector<ext::shared_ptr<BlackCalibrationHelper> > >
    NonstandardSwaption::calibrationBasket(
        const ext::shared_ptr<SwapIndex>& standardSwapBase,
        const ext::shared_ptr<SwaptionVolatilityStructure>& swaptionVolatility,
        const BasketGeneratingEngine::CalibrationBasketType basketType) const {

        ext::shared_ptr<BasketGeneratingEngine> engine =
            ext::dynamic_pointer_cast<BasketGeneratingEngine>(engine_);
        QL_REQUIRE(engine, "engine is not a basket generating engine");
        engine_->reset();
        setupArguments(engine_->getArguments());
        engine_->getArguments()->validate();
        return engine->calibrationBasket(exercise_, standardSwapBase,
                                         swaptionVolatility, basketType);
    }
}