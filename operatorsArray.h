struct operatorInfo operatorsArray[] ={ { "+",      opADD,       sizeof("+") - 1     },
                                        { "-",      opSUB,       sizeof("-") - 1     },
                                        { "*",      opMUL,       sizeof("*") - 1     },
                                        { "/",      opDIV,       sizeof("/") - 1     },
                                        { "!",      opSEPARATOR, sizeof("!") - 1     },
                                        { "(",      opBRACK_ON,    sizeof("(") - 1     },
                                        { ")",      opBRACK_OFF,    sizeof(")") - 1     },
                                        { "wuahh",  opASSIGN,    sizeof("wuahh") - 1 }, //=
                                        { "awa",    opWHILE,     sizeof("awa") - 1   }, //while
                                        { "yaag",   opIF,        sizeof("yaag") - 1  } }; //if

const size_t NUM_OF_OPERATORS = sizeof(operatorsArray) / sizeof(operatorInfo);
