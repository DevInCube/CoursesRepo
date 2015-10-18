
double toDouble(long long ll) {
    return *(double*)&ll;
}

long long toLong(double d) {
    return *(long long *)&d;
}
