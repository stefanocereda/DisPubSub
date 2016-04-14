//srcId wants to receive topic related messages.

message Subscribe_msg
{
    int srcId=-1;//TODO, REALLY NEEDED? the server has the gate, is it sufficient?
    int topic=-1;
};