// (C) 2003, Jan Langer (jan@langernetz.de).
// 
// This material is provided "as is", with absolutely no warranty expressed
// or implied. Any use is at your own risk.
// 
// Permission to use or copy this software for any purpose is hereby granted
// without fee, provided the above notices are retained on all copies.
// Permission to modify the code and to distribute modified code is granted,
// provided the above notices are retained, and a notice that the code was
// modified is included with the above copyright notice.
 
#include <functional>

#include <boost/test/minimal.hpp>
#include <boost/utility/lexicographic.hpp>

int test_main (int, char *[])
{
    using boost::lexicographic;

    lexicographic l1;         // equivalent
    BOOST_CHECK (!l1);
    
    lexicographic l2 (l1);    // equivalent
    BOOST_CHECK (!l2);
    BOOST_CHECK (l1 == l2);

    l2 = l1;
    BOOST_CHECK (!l2);
    BOOST_CHECK (l2 == l1);
    
    l2 (3, 6);                // less
    BOOST_CHECK (l2);
    BOOST_CHECK (l2.result () == lexicographic::minus);
    BOOST_CHECK (lexicographic::minus == l2.result ());
    BOOST_CHECK (l2.result () != lexicographic::equivalent);
    BOOST_CHECK (lexicographic::equivalent != l2.result ());
    BOOST_CHECK (l1 != l2);

    lexicographic l3 (3.0, 1.0);  // greater
    BOOST_CHECK (!l3);
    BOOST_CHECK (l3 != l1);
    
    for (int i = 1; i <= 3; ++i)
        for (int j = 1; j <= 3; ++j)
            for (int k = 1; k <= 3; ++k)
            {
                lexicographic l4;
                l4 (i, 2) (j, 2) (k, 2);

                if (i < 2)
                    BOOST_CHECK (l4);
                else if (i > 2)
                    BOOST_CHECK (!l4);
                else if (j < 2)
                    BOOST_CHECK (l4);
                else if (j > 2)
                    BOOST_CHECK (!l4);
                else if (k < 2)
                    BOOST_CHECK (l4);
                else
                    BOOST_CHECK (!l4);
            }

    lexicographic l5;
    l5 (1, 1, std::greater <int> ()) (2, 3);
    BOOST_CHECK (l5);
    
    lexicographic l6;
    l6 (1, 1, std::greater <int> ()) (2, 3, std::greater <int> ());
    BOOST_CHECK (!l6);
    
    lexicographic l7;
    l7 (1, 1) (2, 3, std::greater <int> ());
    BOOST_CHECK (!l7);

    return 0;
}    
