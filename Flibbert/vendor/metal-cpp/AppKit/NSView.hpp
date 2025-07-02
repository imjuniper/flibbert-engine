/*
 *
 * Copyright 2020-2021 Apple Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//-------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// AppKit/NSView.hpp
//
//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#pragma once

//-------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "AppKitPrivate.hpp"
#include <Foundation/NSObject.hpp>
#include <CoreGraphics/CGGeometry.h>

/* Start Juni edit */
namespace CA {
	class MetalLayer;
}
/* End Juni edit */

namespace NS
{
	class View : public NS::Referencing< View >
	{
		public:
			View*		init( CGRect frame );
			/* Start Juni edit */
			void		setLayer ( CA::MetalLayer* layer );
			void		setOpaque ( bool opaque );
			void		setWantsLayer ( bool wantsLayer );
			/* End Juni edit */
	};
}


_NS_INLINE NS::View* NS::View::init( CGRect frame )
{
	return Object::sendMessage< View* >( _APPKIT_PRIVATE_CLS( NSView ), _APPKIT_PRIVATE_SEL( initWithFrame_ ), frame );
}

/* Start Juni edit */
_NS_INLINE void NS::View::setLayer( CA::MetalLayer* layer )
{
	return Object::sendMessage< void >( this, _APPKIT_PRIVATE_SEL( setLayer_ ), layer );
}

_NS_INLINE void NS::View::setOpaque( bool opaque )
{
	return Object::sendMessage< void >( this, _APPKIT_PRIVATE_SEL( setOpaque_ ), opaque );
}

_NS_INLINE void NS::View::setWantsLayer( bool wantsLayer )
{
	return Object::sendMessage< void >( this, _APPKIT_PRIVATE_SEL( setWantsLayer_ ), wantsLayer );
}
/* End Juni edit */