//=============================================================================================================
/**
* @file     main.cpp
* @author   Christoph Dinh <chdinh@nmr.mgh.harvard.edu>;
*           Matti Hamalainen <msh@nmr.mgh.harvard.edu>
* @version  1.0
* @date     July, 2012
*
* @section  LICENSE
*
* Copyright (C) 2012, Christoph Dinh and Matti Hamalainen. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that
* the following conditions are met:
*     * Redistributions of source code must retain the above copyright notice, this list of conditions and the
*       following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
*       the following disclaimer in the documentation and/or other materials provided with the distribution.
*     * Neither the name of the Massachusetts General Hospital nor the names of its contributors may be used
*       to endorse or promote products derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MASSACHUSETTS GENERAL HOSPITAL BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*
* @brief    Implements the main() application function.
*
*/


//*************************************************************************************************************
//=============================================================================================================
// INCLUDES
//=============================================================================================================

#include <fs/label.h>
#include <fs/surface.h>
#include <fs/annotation_set.h>

#include <inverse/minimumNorm/minimumnorm.h>

#include <disp3D/labelview.h>



#include <iostream>


//*************************************************************************************************************
//=============================================================================================================
// QT INCLUDES
//=============================================================================================================

#include <QGuiApplication>


//*************************************************************************************************************
//=============================================================================================================
// USED NAMESPACES
//=============================================================================================================


using namespace DISP3DLIB;
using namespace FSLIB;


//*************************************************************************************************************
//=============================================================================================================
// MAIN
//=============================================================================================================

//=============================================================================================================
/**
* The function main marks the entry point of the program.
* By default, main has the storage class extern.
*
* @param [in] argc (argument count) is an integer that indicates how many arguments were entered on the command line when the program was started.
* @param [in] argv (argument vector) is an array of pointers to arrays of character objects. The array objects are null-terminated strings, representing the arguments that were entered on the command line when the program was started.
* @return the value that was set to exit() (which is 0 if exit() is called via quit()).
*/
int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    //########################################################################################
    // Source Estimate
    QFile t_fileFwd("./MNE-sample-data/MEG/sample/sample_audvis-meg-eeg-oct-6-fwd.fif");
    QFile t_fileCov("./MNE-sample-data/MEG/sample/sample_audvis-cov.fif");
    QFile t_fileEvoked("./MNE-sample-data/MEG/sample/sample_audvis-ave.fif");

    double snr = 3.0;
    double lambda2 = 1.0 / pow(snr, 2);
    QString method("dSPM"); //"MNE" | "dSPM" | "sLORETA"

    // Load data
    fiff_int_t setno = 0;
    QPair<QVariant, QVariant> baseline(QVariant(), 0);
    FiffEvoked evoked(t_fileEvoked, setno, baseline);
    if(evoked.isEmpty())
        return 1;

    MNEForwardSolution t_Fwd(t_fileFwd);
    if(t_Fwd.isEmpty())
        return 1;

    AnnotationSet t_annotationSet("./MNE-sample-data/subjects/sample/label/lh.aparc.a2009s.annot", "./MNE-sample-data/subjects/sample/label/rh.aparc.a2009s.annot");

    FiffCov noise_cov(t_fileCov);

    // regularize noise covariance
    noise_cov = noise_cov.regularize(evoked.info, 0.05, 0.05, 0.1, true);

    //
    // Cluster forward solution;
    //
    MNEForwardSolution t_clusteredFwd = t_Fwd.cluster_forward_solution(t_annotationSet, 40);

    //
    // make an inverse operators
    //
    FiffInfo info = evoked.info;

    MNEInverseOperator inverse_operator(info, t_clusteredFwd, noise_cov, 0.2f, 0.8f);

    //
    // Compute inverse solution
    //
    MinimumNorm minimumNorm(inverse_operator, lambda2, method);
    SourceEstimate sourceEstimate = minimumNorm.calculateInverse(evoked);

    if(sourceEstimate.isEmpty())
        return 1;

    //Source Estimate end
    //########################################################################################




    Annotation t_annot("./MNE-sample-data/subjects/sample/label/lh.aparc.a2009s.annot");
    Surface t_surf("./MNE-sample-data/subjects/sample/surf/lh.white");

    QList<Label> t_qListLabels;
    QList<RowVector4i> t_qListRGBAs;

    t_annot.toLabels(t_surf, t_qListLabels, t_qListRGBAs);

    LabelView view(t_surf, t_qListLabels, t_qListRGBAs);

    if (view.stereoType() != QGLView::RedCyanAnaglyph)
        view.camera()->setEyeSeparation(0.3f);
    QStringList args = QCoreApplication::arguments();
    int w_pos = args.indexOf("-width");
    int h_pos = args.indexOf("-height");
    if (w_pos >= 0 && h_pos >= 0)
    {
        bool ok = true;
        int w = args.at(w_pos + 1).toInt(&ok);
        if (!ok)
        {
            qWarning() << "Could not parse width argument:" << args;
            return 1;
        }
        int h = args.at(h_pos + 1).toInt(&ok);
        if (!ok)
        {
            qWarning() << "Could not parse height argument:" << args;
            return 1;
        }
        view.resize(w, h);
    }
    else
    {
        view.resize(800, 600);
    }
    view.show();




    //Push Estimate


    view.pushSourceEstimate(sourceEstimate);












    return a.exec();
}