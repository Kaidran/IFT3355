#include "MarchingTetrahedra.h"
#include <QtOpenGL>

MarchingTetrahedra::MarchingTetrahedra( const BoundingBox& boundingBox, unsigned int nbCubeX, unsigned int nbCubeY, unsigned int nbCubeZ )
    : _boundingBox( boundingBox )
{
    QVector3D boxExtent = boundingBox.maximum() - boundingBox.minimum();

    _nbCubes[0] = nbCubeX;
    _nbCubes[1] = nbCubeY;
    _nbCubes[2] = nbCubeZ;
    _cubeSize[0] = boxExtent.x() / _nbCubes[0];
    _cubeSize[1] = boxExtent.y() / _nbCubes[1];
    _cubeSize[2] = boxExtent.z() / _nbCubes[2];

    // Allocate vertex value and position vector
    unsigned int nbCubes = ( nbCubeX + 1 ) * ( nbCubeY + 1 ) * ( nbCubeZ + 1 );
    _vertexValues.resize( nbCubes );
    _vertexNormals.resize( nbCubes );
    _vertexPositions.resize( nbCubes );

    computeVertexPositions();
}

void MarchingTetrahedra::render( const QMatrix4x4& transformation, GLShader& shader, ImplicitSurface& implicitSurface )
{

    // Calculez les valeurs et les normales aux sommets en appelant 'computeVertexInfo' avant de faire le rendu de chaque cube de
    // la grille à l'aide de la fonction 'renderCube'. Une fois fait, appelez 'renderTriangles' pour faire le rendu des triangles.

    // BEGIN TODO //
    _nbGLVertices = 0;
    computeVertexInfo(implicitSurface);
    // ... //
    for ( unsigned int z=0 ; z<_nbCubes[2]; ++z )
        for ( unsigned int y=0 ; y<_nbCubes[1]; ++y )
            for ( unsigned int x=0 ; x<_nbCubes[0]; ++x)
            {
                renderCube( x, y, z );
            }
    renderTriangles(transformation, shader);
    // END TODO //

}

void MarchingTetrahedra::computeVertexPositions()
{   
    unsigned int currentVertex = 0;

	// Precompute the position of each vertex of the grid
    for ( unsigned int z=0 ; z<_nbCubes[2]+1 ; ++z )
        for ( unsigned int y=0 ; y<_nbCubes[1]+1 ; ++y )
            for ( unsigned int x=0 ; x<_nbCubes[0]+1 ; ++x, ++currentVertex )
                _vertexPositions[currentVertex] = vertexPosition( x, y, z );
}

void MarchingTetrahedra::computeVertexInfo( ImplicitSurface& implicitSurface )
{

    // Pour chaque sommet de la grille, remplir les variables membres '_vertexValues' et '_vertexNormals' à l'aide de la position du
    // vertex '_vertexPositions' et de la classe 'implicitSurface'. Notez que les tableaux sont indexés par un seul nombre ... Notez
    // également qu'il y a (_nbCubes[0]+1)x(_nbCubes[1]+1)x(_nbCubes[2]+1) sommets dans la grille.

    // BEGIN TODO //
    unsigned int currentVertex = 0;
    QVector3D normal;
    float value;
    // Precompute the position of each vertex of the grid
    for ( unsigned int z=0 ; z<_nbCubes[2]+1 ; ++z )
        for ( unsigned int y=0 ; y<_nbCubes[1]+1 ; ++y )
            for ( unsigned int x=0 ; x<_nbCubes[0]+1 ; ++x, ++currentVertex )
            {
                implicitSurface.surfaceInfo(_vertexPositions[currentVertex], value, normal);
                _vertexValues[currentVertex] = value;
                _vertexNormals[currentVertex] = normal;
            }

    // ... //

    // END TODO //

}

void MarchingTetrahedra::renderCube( unsigned int x, unsigned int y, unsigned int z )
{

    // Divisez votre cube en six tétraèdres en utilisant les sommets du cube, et faire appel à 'renderTetrahedron'
    // pour le rendu de chacun d'eux

    // BEGIN TODO //

    //compupte vertices
    auto p000 = vertexIndex(x,y,z);
    auto p100 = vertexIndex(x+1,y,z);
    auto p010 = vertexIndex(x,y+1,z);
    auto p001 = vertexIndex(x,y,z+1);
    auto p110 = vertexIndex(x+1,y+1,z);
    auto p101 = vertexIndex(x+1,y,z+1);
    auto p011 = vertexIndex(x,y+1,z+1);
    auto p111 = vertexIndex(x+1,y+1,z+1);

    //compute tetrahedrons
    renderTetrahedron(p000,p100,p110,p111);
    renderTetrahedron(p000,p100,p101,p111);
    renderTetrahedron(p000,p101,p111,p001);
    renderTetrahedron(p000,p110,p111,p010);
    renderTetrahedron(p000,p111,p011,p001);
    renderTetrahedron(p000,p111,p011,p010);

    // ... //

    // END TODO //

}

void MarchingTetrahedra::renderTetrahedron( unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4 )
{

    // En utilisant les valeurs aux sommets, voyez dans quel cas de rendu vous vous trouvez puis faites appel
    // à 'renderTriangle' ou 'renderQuad' dépendant du cas

    // BEGIN TODO
    auto sign_p1 = _vertexValues[p1] < 0 ? -1 : 1;
    auto sign_p2 = _vertexValues[p2] < 0 ? -1 : 1;
    auto sign_p3 = _vertexValues[p3] < 0 ? -1 : 1;
    auto sign_p4 = _vertexValues[p4] < 0 ? -1 : 1;
    auto sum = sign_p1+sign_p2+sign_p3+sign_p4;

    switch(abs(sum))
    {
    // if +- 2, then there is a triangle
    case 2:
        if (sign_p1 == sign_p2 && sign_p2 == sign_p3) // then p4
            renderTriangle(p4,p1,p2,p3);
        else if (sign_p1 == sign_p2 && sign_p2 == sign_p4) // then p3
            renderTriangle(p3,p1,p2,p4);
        else if (sign_p1 == sign_p3 && sign_p3 == sign_p4) // then p2
            renderTriangle(p2,p1,p3,p4);
        else //p1
            renderTriangle(p1,p2,p3,p4);
        break;
    case 0:
        if (sign_p1 == sign_p2)
            renderQuad(p1,p2,p3,p4);
        else if (sign_p1 == sign_p3)
            renderQuad(p1,p3,p2,p4);
        else
            renderQuad(p1,p4,p2,p3);
        break;
    default:
        return;
    }
    // ... //

    // END TODO

}

// Helper function. Linear interpolation between two vertices of a triangle.
QVector3D MarchingTetrahedra::interpolate(unsigned int p1, unsigned int p2, QVector3D& norm)
{
    norm = QVector3D();

    QVector3D pt1 = _vertexPositions[p1];
    QVector3D pt2 = _vertexPositions[p2];

    auto v1 = _vertexValues[p1];
    auto v2 = _vertexValues[p2];

    QVector3D n1 = _vertexNormals[p1];
    QVector3D n2 = _vertexNormals[p2];

    auto t = -v1 / (v2-v1);
    auto p = t*(pt2 - pt1) + pt1;
    norm = (t*(n2 - n1) + n1).normalized();

    return p;

}

void MarchingTetrahedra::renderTriangle( unsigned int in1, unsigned int out2, unsigned int out3, unsigned int out4 )
{

    // Calculez l'interpolation des valeurs et des normales pour les arêtes dont les sommets sont de signes
    // différents. N'oubliez pas de normaliser vos normales. Une fois fait, ajoutez le triangle à la liste des triangles
    // à affichier en utilisant la méthode 'addTriangle'

    // BEGIN TODO
    QVector3D n0;
    QVector3D n1;
    QVector3D n2;

    QVector3D p0 = interpolate(in1, out2, n0);
    QVector3D p1 = interpolate(in1, out3, n1);
    QVector3D p2 = interpolate(in1, out4, n2);
    // ... //

    addTriangle(p0,p1,p2,n0,n1,n2);
    // END TODO

}

void MarchingTetrahedra::renderQuad( unsigned int in1, unsigned int in2, unsigned int out3, unsigned int out4 )
{

    // Calculez l'interpolation des valeurs et des normales pour les arêtes dont les sommets sont de signes
    // différents. Vous aurez quatre sommets. Séparez le quadrilatère en deux triangles, puis ajoutez les à
    // la liste des triangles à affichier en utilisant la méthode 'addTriangle'

    // BEGIN TODO
    QVector3D n0;
    QVector3D n1;
    QVector3D n2;
    QVector3D n3;

    // triangles p0,p1,p2 && p2,p3,p0
    QVector3D p0 = interpolate(in1, out3, n0);
    QVector3D p1 = interpolate(in1, out4, n1);
    QVector3D p2 = interpolate(in2, out3, n2);
    QVector3D p3 = interpolate(in2, out4, n3);
    // ... //

    addTriangle(p0, p1, p2, n0, n1, n2);
    addTriangle(p2, p3, p1, n2, n3, n1);
    // END TODO


}

QVector3D MarchingTetrahedra::vertexPosition( unsigned int x, unsigned int y, unsigned int z ) const
{
    return _boundingBox.minimum() + QVector3D( x * _cubeSize[0], y * _cubeSize[1], z * _cubeSize[2] );
}

unsigned int MarchingTetrahedra::vertexIndex( unsigned int x, unsigned int y, unsigned int z ) const
{
    return z * ( _nbCubes[0] + 1 ) * ( _nbCubes[1] + 1 ) + y * ( _nbCubes[0] + 1 ) + x;
}

void MarchingTetrahedra::addTriangle( const QVector3D& p0, const QVector3D& p1, const QVector3D& p2,
                                      const QVector3D& n0, const QVector3D& n1, const QVector3D& n2 )
{
    if ( _glVertices.size() <= _nbGLVertices )
    {
        _glVertices.resize( _glVertices.size() + 192 );
        _glNormals.resize( _glNormals.size() + 192 );
    }

    _glVertices[_nbGLVertices+0] = p0;
    _glVertices[_nbGLVertices+1] = p1;
    _glVertices[_nbGLVertices+2] = p2;
    _glNormals[_nbGLVertices+0] = n0;
    _glNormals[_nbGLVertices+1] = n1;
    _glNormals[_nbGLVertices+2] = n2;
    _nbGLVertices += 3;
}

void MarchingTetrahedra::renderTriangles( const QMatrix4x4& transformation, GLShader& shader )
{
    shader.setGlobalTransformation( transformation );

    shader.enableVertexAttributeArray();
    shader.enableNormalAttributeArray();
    shader.setVertexAttributeArray( _glVertices.data() );
    shader.setNormalAttributeArray( _glNormals.data() );

    glDrawArrays( GL_TRIANGLES, 0, _nbGLVertices );

    shader.disableVertexAttributeArray();
    shader.disableNormalAttributeArray();
}
