struct pt{
    ll x, y;

    pt(){}

    pt(ll _x, ll _y):x(_x), y(_y){}

    pt operator+(pt p){
        return pt(x + p.x, y + p.y);
    }

    pt operator-(pt p){
        return pt(x - p.x, y - p.y);
    }

    ll cross(pt p){
        return x * p.y - y * p.x;
    }

    ll dot(pt p){
        return x * p.x + y * p.y;
    }

    ll cross(pt a, pt b){
        return (a - *this).cross(b - *this);
    }

    ll dot(pt a, pt b){
        return (a - *this).dot(b - *this);
    }

    ll sqrLen(){
        return this->dot(*this);
    }
};

bool lexComp(pt l, pt r){
    return l.x < r.x || (l.x == r.x && l.y < r.y);
}

int sgn(ll val){
    return val > 0 ? 1 : (val == 0 ? 0 : -1);
}

bool pointsInTriangle(pt a, pt b, pt c, pt point){
    ll s1 = abs(a.cross(b, c));
    ll s2 = abs(point.cross(a, b)) + abs(point.cross(b, c)) + abs(point.cross(c, a));

    return s1 == s2;
}

bool pointInConvexPolygon(pt point, vector< pt > points){
    int n = points.size();

    pt p1 = points[1];
    pt p0 = points[0];
    pt pn = points[n - 1];
    
    if((p1 - p0).cross(point - p0) != 0 && sgn((p1 - p0).cross(point - p0)) != sgn((p1 - p0).cross(pn - p0))){
        return false;
    }
    
    if((pn - p0).cross(point - p0) != 0 && sgn((pn - p0).cross(point - p0)) != sgn((pn - p0).cross(p1 - p0))){
        return false;
    }

    if((p1 - p0).cross(point - p0) == 0){
        return (p1 - p0).sqrLen() >= (point - p0).sqrLen();
    }

    int lo = 0;
    int hi = n - 2;
    int r = -1;

    while(lo <= hi){
        int mid = (lo + hi) >> 1;

        if((point - p0).cross(points[mid] - p0) <= 0){
            r = mid;
            lo = mid + 1;
        }else{
            hi = mid - 1;
        }
    }

    assert(r != -1);

    return pointsInTriangle((points[r] - p0), points[(r + 1) % n] - p0, pt(0, 0), point - p0);
}
