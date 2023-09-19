#include <util/delay.h>
#include <util/atomic.h>
#include <avr/pgmspace.h>
#include <avr/io.h>
#include <cstdio>
#include <cmath>
#include <vector>
#include "ode_newton.h"
#include "test_systems.h"

extern void USART_Init();

#define F(s) PSTR(s)
#define printfln( format, ... ) printf( ( const char * ) ( format "\n" ), ##__VA_ARGS__ )

template<class T>
void test_sys( T sys )
{
    // Integration time.
    double tint = 3;

    // Small time step.
    double dt = 0.001;

    // Error "tolerance" sort of:
    double tol = 0.1;

    // Exact solutions for oscilator 1.
    double y0ex = exp( sin( tint * tint ) ),
           y1ex = exp( cos( tint * tint ) );

    double y0er, y1er;

    // Integrate the system.
    sys.solve_fixed( tint, dt );

    // Calculate errors.
    y0er = fabs( sys.get_sol(0) - y0ex );
    y1er = fabs( sys.get_sol(1) - y1ex );

    // Check the errors.
    printf_P( F( "%s:\n" ), sys.get_method() );
    printf_P( F( "  y0 error = %12e" ), y0er );

    if ( y0er > tol )
    {
        printf_P( F( " <---- DANGER!!!\n" ) );
        exit( EXIT_FAILURE );
    }
    else
    {
        printf_P( F( "\n" ) );
    }

    printf_P( F( "  y1 error = %12e" ), y1er );

    if ( y1er > tol )
    {
        printf_P( F( " <---- DANGER!!!\n" ) );
        exit( EXIT_FAILURE );
    }
    else
    {
        printf_P( F( "\n" ) );
    }
}


void test_adapt()
{
    printf_P( F( "test_adapt:\n" ) );

    // Choose an integration time.
    double tint = 5;

    // Choose which system and solver to use.
    Osc2<OdeDoPri54> sys;

    // Choose error tolerance.
    sys.set_tol( 1e-6 );

    printf_P( F( "Solving system '%s' with method '%s'\n" ), sys.get_name(), sys.get_method() );

    sys.solve_adaptive( tint, tint / 10.0, "out", 1 );

    printf_P( F( "%lu function evaluations, %lu steps, %lu rejected steps\n" ), sys.get_neval(), sys.get_nstep(), sys.get_nrej() );
}


// Function to find root of.
void f_f( double * x, double * y )
{
    y[0] = 3 * x[0] - cos( x[1] * x[2] ) - 0.5;

    y[1] = x[0] * x[0] - 81.0 * ( x[1] + 0.1 ) * ( x[1] + 0.1 ) + sin( x[2]) + 1.06;

    y[2] = exp( -x[0] * x[1] ) + 20.0 * x[2] + ( 10 * M_PI - 3.0 ) / 3.0;
}


// Jacobian of f().
void f_J( double * x, double **J )
{
    J[0][0] = 3.0;
    J[0][1] = sin( x[1] * x[2] ) * x[1];
    J[0][2] = sin( x[1] * x[2] ) * x[2];

    J[1][0] = 2 * x[0];
    J[1][1] = -162.0 * ( x[1] + 0.1 );
    J[1][2] = -cos( x[2] );

    J[2][0] = -x[1] * exp( -x[0] * x[1] );
    J[2][1] = -x[0] * exp( -x[0] * x[1] );
    J[2][2] = 20.0;
}

class TestNewton : public OdeNewton
{
public:
    TestNewton() : OdeNewton (3) {}
    void f_Newton( double * x, double *y ) { f_f(x, y); }
    void J_Newton( double * x, double **J ) { f_J(x, J); }
};

void test_newton()
{
    // System and solver.
    TestNewton sys;

    sys.set_modified( true );

    // Initial values.
    auto x = std::make_unique<double[]>(3);

    // Print the solution.
    printf_P( F( "Exact solution:\n" ) );

    printf_P( F( "x0 = %16g\nx1 = %16g\nx2 = %16g\n" ), 0.5, 0.0, -0.52359877 );

    //set initial values
    x[0] = 1.0; x[1] = 1.0; x[2] = 1.0;

    // Solve with class functions.
    sys.solve_Newton( x.get() );

    // Print results.
    printf_P( F( "Newton solution:\n" ) );
    printf_P( F( "x0 = %16g\nx1 = %16g\nx2 = %16g\n" ), x[0], x[1], x[2] );
    printf_P( F( "nJLU = %lu\n" ), sys.get_nJLU() );
}


void test_snaps()
{
    // Время интегрирования.
    double tint = 5;

    // Длительность интервала.
    double dt = tint / 1000.0;

    // Сhoose the number of snaps.
    int nsnap = 5;

    // Choose which system and solver to use.
    Osc1<OdeVern65> sys;

    auto tsnap = std::make_unique<double[]>( nsnap );

    tsnap[0] = 1.1;
    tsnap[1] = 2.22;
    tsnap[2] = 3;
    tsnap[3] = 3.14;
    tsnap[4] = 4.9;

    printf_P( F( "Solving system '%s' with method '%s'\n" ), sys.get_name(), sys.get_method() );

    // Solve with evenly spaced snaps.
    //sys.solve_adaptive( tint, dt, nsnap, "out" );

    // Solve with snap times from tsnap.
    sys.solve_adaptive( dt, tsnap.get(), nsnap, "out" );

    printf( F( "%lu function evaluations, %lu steps\n" ), sys.get_neval(), sys.get_nstep() );
}


void test_all()
{
    printf_P( F( "test all solvers:\n" ) );

    // Integrate all the systems and check their errors.
    test_sys( Osc2<OdeEuler>() );
    test_sys( Osc2<OdeTrapz>() );
    test_sys( Osc2<OdeSsp3>() );
    test_sys( Osc2<OdeRKF32>() );
    test_sys( Osc2<OdeRK4>() );
    test_sys( Osc2<OdeRK43>() );
    test_sys( Osc2<OdeRKCK>() );
    //test_sys( Osc2<OdeDoPri54>() );
    //test_sys( Osc2<OdeVern65>() );
    //test_sys( Osc2<OdeVern76>() );
    //test_sys( Osc2<OdeDoPri87>() );
    //test_sys( Osc2<OdeVern98>() );
    //test_sys( Osc2<OdeGRK4A>() );
    //test_sys( Osc2<OdeROW6A>() );
    //test_sys( Osc2<OdeBackwardEuler>() );
    //test_sys( Osc2<OdeGauss6>() );
    //test_sys( Osc2<OdeLobattoIIIC6>() );
    //test_sys( Osc2<OdeRadauIIA5>() );
    //test_sys( Osc2<OdeGeng5>() );
    //test_sys( Osc2<OdeSDIRK43>() );
}


/**
 * \brief   Точка входа в программу.
 *
 */
int main()
{
    // Настройка USART.
    USART_Init();

    //test_adapt();
    //test_newton();
    //test_snaps();
    test_all();

    while (1) {}
}
