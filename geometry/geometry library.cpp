#define LD long double
struct pt{
    LD x,y;
    pt operator+(pt p){return {x+p.x,y+p.y};}
    pt operator-(pt p){return {x-p.x,y-p.y};}
    pt operator*(LD m){return {x*m,y*m};}
    pt operator/(LD m){return {x/m,y/m};}
    bool operator==(pt other){return x==other.x && y==other.y;}
    bool operator!=(pt other){return x!=other.x || y!=other.y;}
    void read(){
        cin>>x>>y;
    }
};
template <typename T> int sgn(T x){
    return (T(0)<x)-(x<T(0));
}
LD sq(pt p) {
    return p.x*p.x + p.y*p.y;
}
double abs(pt p) {
    return sqrt(sq(p));
}
ostream& operator<<(ostream& os, pt p){
    return os << "(" <<p.x<<","<<p.y<<")";
}
pt translate(pt p,pt v){ // translates p by v
    return p+v;
}
pt scale(pt p,pt center,LD factor){ // scale p by factor around center
    return center+(p-center)*factor;
}
pt rotation(pt p,LD ang){ // rotate p by ang
    return {p.x*cos(ang)-p.y*sin(ang),p.x*sin(ang)+p.y*cos(ang)};
}
pt perp(pt p){ // rotation by 90 degrees
    return {-p.y,p.x};
}
LD dot(pt v,pt w){ // dot product of v,w
    return v.x*w.x+v.y*w.y;
}
bool isPerp(pt v,pt w){ // check whether v,w are perpendicular
    return dot(v,w)==0;
}
LD angle(pt v,pt w){ // angle between v,w
    double cosTheta = dot(v,w) / abs(v) / abs(w);
    return acos(max(-1.0, min(1.0, cosTheta)));
}
LD cross(pt v,pt w){ // calculates the cross product of v,w
    return v.x*w.y-v.y*w.x;
}
LD orient(pt a,pt b,pt c){ // positive if C is on the left of AB, negative if C is on the right side
    return cross(b-a,c-a);
}
bool isPointInside(pt a,pt b,pt c,pt point){ // check whether point lies within AB, AC
    assert(orient(a,b,c)!=0); // the must not be collinear
    if (orient(a,b,c)<0)swap(b,c); // b is to the left
    return orient(a,b,point)>=0&&orient(a,c,point)<=0;
}
bool isConvex(vector < pt > p){
    bool hasPos=0,hasNeg=0;
    int n=p.size();
    for (int i=0;i<n;i++){
        LD o = orient(p[i],p[(i+1)%n],p[(i+2)%n]);
        if (o>0)hasPos=1;
        if (o<0)hasNeg=1;
    }
    return !(hasNeg&&hasPos);
}
bool half(pt p){ // check if point is on the positive side of y's (for polar sorting)
    assert(p.x!=0||p.y!=0);
    return p.y>0 || (p.y==0&&p.x<0);
}
void polarSort(vector <pt> &vec){ // polar sort, tie breaker is magnitude
    sort(vec.begin(),vec.end(),[](pt v,pt w){
         return make_tuple(half(v),0,sq(v))<make_tuple(half(w),cross(v,w),sq(w));
         });
}

struct line{
    pt v; //direction vector
    LD c; // offset
    line(pt v,LD c): v(v),c(c){} // direct
    line(LD a,LD b,LD c): v({b,-a}) , c(c){} // from equation ax+by=c
    line(pt p,pt q): v(p-q),c(cross(v,p)){} // from point p,q

    // - these work with LD = int
    LD side(pt p){ return cross(v,p)-c; } // decide on which side the point p lies (left is positive)
    long double dist(pt p){ return abs(side(p))/abs(v); } // distance from point
    line perpThrough(pt p){ return {p,p+perp(v)}; } // another line perpendicular through point p
    bool cmpProj(pt p, pt q){ return dot(v,p)<dot(v,q); } // sort points in order they are appearing on the line
    line translate(pt t){ return {v, c + cross(v,t)}; } //translate line with vector t

    // - these require LD = double
    line shiftLeft(LD dist){ return {v, c + dist*abs(v)};}
    double sqDist(pt p) {return side(p)*side(p) / (double)sq(v);}
    pt proj(pt p){ return p - perp(v)*side(p)/sq(v); } // project point on the line
    pt refl(pt p){ return p - perp(v)*2*side(p)/sq(v); } // reflect the point over the line
};

bool inter(line l1, line l2,pt &out) {  //out is the point of intersection
    LD d=cross(l1.v,l2.v);
    if (d==0)return false;
    out=(l2.v*l1.c - l1.v*l2.c)/d; // requires floating-point coordinates
    return true;
}
line bisector(line l1, line l2,bool interior) {
    assert(cross(l1.v, l2.v)!=0); // l1 and l2 cannot be parallel!
    double sign=interior?1:-1;
    return {l2.v/abs(l2.v)+l1.v/abs(l1.v)*sign , l2.c/abs(l2.v)+l1.c/abs(l1.v)*sign};
}

// segment related stuff:
bool inDisk(pt a,pt b,pt p) { // returns true if point p is in the circle formed by ab
    return dot(a-p,b-p)<=0;
}
bool onSegment(pt a, pt b, pt p) {
    return orient(a,b,p)==0&&inDisk(a,b,p);
}
bool properInter(pt a,pt b,pt c,pt d,pt &out){ // finds intersection of two segments
    LD oa=orient(c,d,a),
    ob=orient(c,d,b),
    oc=orient(a,b,c),
    od=orient(a,b,d);
    // Proper intersection exists iff opposite signs
    if (oa*ob<0 && oc*od<0) {
        out=(a*ob-b*oa)/(ob-oa);
        return true;
    }
    return false;
}
LD segPointDistance(pt a,pt b,pt p) {
    if (a!=b){
        line l(a,b);
        if (l.cmpProj(a,p)&&l.cmpProj(p,b)) return l.dist(p);
    }
    return min(abs(p-a), abs(p-b));
}
LD segSegDistance(pt a, pt b, pt c, pt d) {
    pt dummy;
    if (properInter(a,b,c,d,dummy))return 0;
    return min({segPointDistance(a,b,c), segPointDistance(a,b,d),segPointDistance(c,d,a), segPointDistance(c,d,b)});
}

// polygon related stuff:
LD areaTriangle(pt a,pt b,pt c) {
    return abs(cross(b-a,c-a))/2.0;
}
LD areaPolygon(vector<pt> p) {
    LD area=0.0;
    for (int i=0,n=p.size();i<n;i++) {
        area += cross(p[i],p[(i+1)%n]);
    }
    return abs(area)/2.0;
}

bool above(pt a,pt p) {// true if P at least as high as A
    return p.y>=a.y;
}
bool crossesRay(pt a, pt p, pt q) { // check if [PQ] crosses ray from A
    return (above(a,q)-above(a,p))*orient(a,p,q)>0;
}
bool inPolygon(vector<pt> p,pt a,bool strict=true){ // if strict, returns false when A is on the boundary
    int numCrossings = 0;
    for (int i=0,n=p.size();i<n;i++) {
        if (onSegment(p[i],p[(i+1)%n],a)){
            return !strict;
        }
        numCrossings+=crossesRay(a, p[i], p[(i+1)%n]);
    }
    return numCrossings&1;
}

// circles stuff:
pt circumCenter(pt a, pt b, pt c) { // center of circle that passes through a,b,c
    b=b-a,c=c-a; // consider coordinates relative to A
    assert(cross(b,c)!=0); // no circumcircle if A,B,C aligned
    return a+perp(b*sq(c)-c*sq(b))/cross(b,c)/2;
}
int circleLineInter(pt o,LD r,line l,pair<pt,pt> &out){
    LD h2=r*r-l.sqDist(o);
    if (h2>=0) { // the line touches the circle
        pt p=l.proj(o); // point P
        pt h=l.v*sqrt(h2)/abs(l.v);
        out={p-h,p+h};
    }
    return 1+sgn(h2);
}
int circleCircleInter(pt o1,LD r1, pt o2,LD r2, pair<pt,pt> &out) {
    pt d=o2-o1;
    LD d2=sq(d);
    if (d2==0) {
        assert(r1 != r2);
        return 0;
    } // concentric circles
    LD pd=(d2+r1*r1-r2*r2)/2; // = |O_1P| * d
    LD h2=r1*r1-pd*pd/d2; // = hˆ2
    if (h2 >= 0) {
        pt p = o1+d*pd/d2,h=perp(d)*sqrt(h2/d2);
        out={p-h, p+h};
    }
    return 1+sgn(h2);
}
