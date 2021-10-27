#pragma once

#ifdef test
#define SA_Private public
#define SA_Protected public

#else
#define SA_Private private
#define SA_Protected protected
#endif // test